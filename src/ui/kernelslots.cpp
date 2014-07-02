#include "kernel.h"

#include <gnome-keyring-1/gnome-keyring.h>
#include <QStatusBar>

extern const GnomeKeyringPasswordSchema *GNOME_KEYRING_NETWORK_PASSWORD;

int Engine::Kernel::getSilentConnection(std::string ip)
{
    if (m_connections.find(ip) == m_connections.end()) {
        CIMClient *client = NULL;
        GnomeKeyringAttributeList *list = gnome_keyring_attribute_list_new();
        GList *res_list;

        gnome_keyring_attribute_list_append_string(list, "server", ip.c_str());

        GnomeKeyringResult res = gnome_keyring_find_items_sync(
                    GNOME_KEYRING_ITEM_NETWORK_PASSWORD,
                    list,
                    &res_list
                    );

        if (res == GNOME_KEYRING_RESULT_NO_MATCH) {
            gnome_keyring_found_list_free(res_list);
            return 1;
        } else if (res != GNOME_KEYRING_RESULT_OK) {
            emit error("Cannot get username and password");
            gnome_keyring_found_list_free(res_list);
            return -1;
        }

        std::string username;
        GnomeKeyringFound *keyring = ((GnomeKeyringFound*) res_list->data);
        guint cnt = g_array_get_element_size(keyring->attributes);
        for (guint i = 0; i < cnt; i++) {
            GnomeKeyringAttribute tmp;
            if (strcmp((tmp = g_array_index(keyring->attributes, GnomeKeyringAttribute, i)).name, "user") == 0) {
                username = gnome_keyring_attribute_get_string(&tmp);
                break;
            }
        }

        gchar* passwd;
        gnome_keyring_find_password_sync(
                    GNOME_KEYRING_NETWORK_PASSWORD,
                    &passwd,
                    "user", username.c_str(),
                    "server", ip.c_str(),
                    NULL
                    );
        gnome_keyring_found_list_free(res_list);

        client = new CIMClient();
        try {
            client->connect(ip, 5988, false, username, passwd);
            m_connections[ip] = client;
            return 0;
        } catch (Pegasus::Exception &ex) {
            emit error(std::string(ex.getMessage().getCString()));
            return -1;
        }
    }

    return 0;
}

void Engine::Kernel::deletePasswd()
{
    std::string id = m_main_window.getPcTreeWidget()->getTree()->selectedItems()[0]->text(0).toStdString();
    deletePasswd(id);
}

void Engine::Kernel::deletePasswd(std::string id)
{
    GnomeKeyringResult res = gnome_keyring_delete_password_sync(
                GNOME_KEYRING_NETWORK_PASSWORD,
                "server", id.c_str(),
                NULL
                );

    if (res != GNOME_KEYRING_RESULT_OK)
        Logger::getInstance()->error("Cannot delete password");
    else
        Logger::getInstance()->info("Password deleted");
}


void Engine::Kernel::enableSpecialButtons()
{
    enableSpecialButtons(true);
}

void Engine::Kernel::enableSpecialButtons(bool state)
{
    QList<QTreeWidgetItem*> list = m_main_window.getPcTreeWidget()->getTree()->selectedItems();
    bool refresh = !list.empty() & m_refreshEnabled & state;
    QPushButton *button = m_main_window.findChild<QPushButton*>("refresh_button");
    button->setEnabled(refresh);
    button = m_main_window.getToolbar()->findChild<QPushButton*>("delete_passwd_button");
    button->setEnabled(refresh);
    button = m_main_window.getToolbar()->findChild<QPushButton*>("shutdown_button");
    button->setEnabled(refresh);
    button = m_main_window.getToolbar()->findChild<QPushButton*>("reboot_button");
    button->setEnabled(refresh);
}

void Engine::Kernel::handleAuthentication(PowerStateValues::POWER_VALUES state)
{
    QTreeWidgetItem* item = m_main_window.getPcTreeWidget()->getTree()->selectedItems()[0];
    std::string ip = item->text(0).toStdString();
    AuthenticationDialog dialog(ip);
    if (dialog.exec()) {
        std::string username = dialog.getUsername();
        std::string passwd = dialog.getPasswd();
        if (username == "" || passwd == "")
            return;

        GnomeKeyringResult res =
                gnome_keyring_store_password_sync(
                    GNOME_KEYRING_NETWORK_PASSWORD,
                    OPENLMI_KEYRING_DEFAULT,
                    ip.c_str(),
                    passwd.c_str(),
                    "user", username.c_str(),
                    "server", ip.c_str(),
                    NULL
                    );

        if (res != GNOME_KEYRING_RESULT_OK) {
            Logger::getInstance()->error("Cannot store password!");
            return;
        }

        boost::thread(boost::bind(&Engine::Kernel::getConnection, this, state));
    } else {
        handleProgressState(1);
        m_main_window.getStatusBar()->clearMessage();
    }
}

void Engine::Kernel::handleConnecting(CIMClient *client, PowerStateValues::POWER_VALUES state)
{
    if (client == NULL) {
        handleProgressState(1);
        return;
    }

    if (state == PowerStateValues::NoPowerSetting) {
        QTabWidget* tab = m_main_window.getProviderWidget()->getTabWidget();
        IPlugin *plugin = (IPlugin*) tab->currentWidget();
        m_main_window.getPcTreeWidget()->setComputerIcon(QIcon(":/computer.png"));

        if (plugin != NULL)
            plugin->refresh(client);
    } else {
        setPowerState(client, state);
        handleProgressState(100);
    }
}

void Engine::Kernel::handleError(std::string message)
{
    m_main_window.getStatusBar()->clearMessage();
    Logger::getInstance()->error(message);
}

void Engine::Kernel::handleInstructionText(std::string text)
{
    m_code_dialog.setText(text, false);
}

void Engine::Kernel::handleProgressState(int state)
{
    QTabWidget *tab = m_main_window.getProviderWidget()->getTabWidget();
    IPlugin *plugin = (IPlugin*) tab->currentWidget();
    if (state == 100) {
        tab->setEnabled(true);
        plugin->setPluginEnabled(true);
        setButtonsEnabled(true);
        m_main_window.getStatusBar()->clearMessage();
        m_bar->hide();
    } else if (state == 0) {
        plugin->setPluginEnabled(false);
        m_bar->setMaximum(0);
        m_bar->show();
    } else {
        tab->setEnabled(true);
        plugin->setPluginEnabled(false);
        enableSpecialButtons(true);
        m_bar->hide();
    }
}

void Engine::Kernel::rebootPc()
{
    QList<QTreeWidgetItem *> items;
    if (!(items = m_main_window.getPcTreeWidget()->getTree()->selectedItems()).isEmpty())
        Logger::getInstance()->info("Rebooting system: "
                                    + m_main_window.getPcTreeWidget()->getTree()->selectedItems()[0]->text(0).toStdString()
                                    );
    m_main_window.getPcTreeWidget()->setComputerIcon(QIcon(":/reboot.png"));

    handleProgressState(0);
    boost::thread(boost::bind(&Engine::Kernel::getConnection, this, PowerStateValues::PowerCycleOffSoft));
}

void Engine::Kernel::refresh()
{
    if (!m_refreshEnabled || m_main_window.getPcTreeWidget()->getTree()->selectedItems().empty())
        return;

    QTabWidget *tab = m_main_window.getProviderWidget()->getTabWidget();
    IPlugin *plugin = (IPlugin*) tab->currentWidget();

    if (plugin == NULL)
        return;

    tab->setEnabled(false);
    setButtonsEnabled(false);
    handleProgressState(0);
    boost::thread(boost::bind(&Engine::Kernel::getConnection, this, PowerStateValues::NoPowerSetting));
}

void Engine::Kernel::resetKeyring()
{
    GnomeKeyringResult res = gnome_keyring_delete_sync(OPENLMI_KEYRING_DEFAULT);
    if (res != GNOME_KEYRING_RESULT_OK) {
        Logger::getInstance()->error("Cannot reset keyring");
        return;
    }
    createKeyring();
}

void Engine::Kernel::saveAsScripts()
{
    m_save_script_path = "";
    saveScripts();
}


void Engine::Kernel::saveScripts()
{
    if (m_save_script_path.empty()) {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::Directory);
        m_save_script_path = dialog.getExistingDirectory().toStdString();
        m_save_script_path += "/" + m_main_window.getPcTreeWidget()->getTree()
                ->selectedItems()[0]->text(0).toStdString();
    }

    plugin_map::iterator it;
    for (it = m_loaded_plugins.begin(); it != m_loaded_plugins.end(); it++)
        it->second->saveScript(m_save_script_path);
}

void Engine::Kernel::setActivePlugin(int index)
{
    int i = 0;
    for (plugin_map::iterator it = m_loaded_plugins.begin(); it != m_loaded_plugins.end(); it++) {
        if (i == index) {
            (*it).second->setActive(true);
            m_code_dialog.setText((*it).second->getInstructionText(), false);
            setButtonsEnabled((*it).second->isRefreshed(), false);
            QPushButton *button = m_main_window.getToolbar()->findChild<QPushButton*>("filter_button");
            if (button != NULL)
                button->setChecked((*it).second->isFilterShown());
        } else
            (*it).second->setActive(false);
        i++;
    }
}

void Engine::Kernel::setEditState(bool state)
{
    PCTreeWidget* tree_widget = m_main_window.getPcTreeWidget();
    m_refreshEnabled = !state;
    m_main_window.getProviderWidget()->setEnabled(!state);
    setButtonsEnabled(!state);
    bool empty = m_main_window.getPcTreeWidget()->getTree()->selectedItems().empty();
    ((QPushButton*) m_main_window.getToolbar()->findChild<QPushButton*>("delete_passwd_button"))->setEnabled(state & !empty);
    ((QPushButton*) m_main_window.getToolbar()->findChild<QPushButton*>("add_button"))->setEnabled(state);
    ((QPushButton*) m_main_window.getToolbar()->findChild<QPushButton*>("remove_button"))->setEnabled(state);
    ((QPushButton*) m_main_window.getToolbar()->findChild<QPushButton*>("discover_button"))->setEnabled(state);
    ((QPushButton*) m_main_window.getToolbar()->findChild<QPushButton*>("edit_button"))->setIcon(
                QIcon(!state ? ":/changes-prevent.png" : ":/changes-allow.png"));

    tree_widget->setEditState(state);
}

void Engine::Kernel::setPluginNoChanges(IPlugin *plugin)
{
    QTabWidget *tab = m_main_window.getProviderWidget()->getTabWidget();
    tab->setTabText(getIndexOfTab(plugin->getLabel()), plugin->getLabel().c_str());
}

void Engine::Kernel::setPluginUnsavedChanges(IPlugin *plugin)
{
    QTabWidget *tab = m_main_window.getProviderWidget()->getTabWidget();
    tab->setTabText(getIndexOfTab(plugin->getLabel()), std::string("* " + plugin->getLabel()).c_str());
}

void Engine::Kernel::showCodeDialog()
{
    QTabWidget *tab = m_main_window.getProviderWidget()->getTabWidget();
    IPlugin *plugin = (IPlugin*) tab->currentWidget();
    m_code_dialog.setText(plugin->getInstructionText(), false);
    m_code_dialog.show();
}

void Engine::Kernel::showFilter()
{
    QPushButton *button = m_main_window.getToolbar()->findChild<QPushButton*>("filter_button");

    QTabWidget *tab = m_main_window.getProviderWidget()->getTabWidget();
    IPlugin *plugin = (IPlugin*) tab->currentWidget();

    if (plugin == NULL) {
        button->setChecked(false);
        return;
    }

    plugin->showFilter(button->isChecked());
}

void Engine::Kernel::shutdownPc()
{
    QList<QTreeWidgetItem *> items;
    if (!(items = m_main_window.getPcTreeWidget()->getTree()->selectedItems()).isEmpty())
        m_main_window.getPcTreeWidget()->setComputerIcon(QIcon(":/shutdown.png"));
        Logger::getInstance()->info("Shutting down system: "
                                    + m_main_window.getPcTreeWidget()->getTree()->selectedItems()[0]->text(0).toStdString()
                                    );

    handleProgressState(0);
    boost::thread(boost::bind(&Engine::Kernel::getConnection, this, PowerStateValues::PowerOffSoftGraceful));
}
