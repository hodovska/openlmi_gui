/* ***** BEGIN LICENSE BLOCK *****
 *   Copyright (C) 2013-2014, Martin Hatina <mhatina@redhat.com>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2.1 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 * ***** END LICENSE BLOCK ***** */

#ifndef PCTREEWIDGET_H
#define PCTREEWIDGET_H

#include "discoverworker.h"
#include "dialogs/systemdetailsdialog.h"
#include "lmi_string.h"
#include "treewidgetitem.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QToolBar>
#include <QTreeWidgetItem>
#include <QWidget>

#ifndef PATTERN_TYPE_FQDN
#   define PATTERN_TYPE_FQDN \
    "(?=^.{1,254}$)(^(?:(?!\\d+\\.)[a-zA-Z0-9_\\-]{1,63}\\." \
    "?)+(?:[a-zA-Z]{2,})$)"
#endif

namespace Ui
{
class PCTreeWidget;
}

/**
 * @brief The PCTreeWidget class
 *
 * Represent list of hosts.
 */
class PCTreeWidget : public QWidget
{
    Q_OBJECT

private:
    bool m_data_of_item_changed;
    bool m_emit_signal;
    bool m_new_item;
    DiscoverWorker *m_worker;
    int m_time_sec;
    QMenu *m_context_menu;
    QPoint m_item_pos;
    QTimer m_timer;
    SystemDetailsDialog m_dialog;
    TreeWidgetItem *m_item_to_show;
    Ui::PCTreeWidget *m_ui;

    bool parentContainsItem(QTreeWidgetItem *parent, String text, QTreeWidgetItem *item = NULL);
    int topLevelNodeCount(String item_name = "");
    String getHostName(String &ip, int &ai_family);
    TreeWidgetItem *addPcToTree(String parent, String text);
    TreeWidgetItem *findTopLevelNode(String item_name);
    void changeDisplayedName(TreeWidgetItem *item);
    void getIp(String &name, String &ipv4, String &ipv6);
    void initContextMenu();
    void loadPcs(String filename);
    void saveAllPcs(String filename);

private slots:
    void addDiscoveredPcsToTree(std::list<String> *pc);
    void addGroup();
    void deleteGroup();
    void itemSelectionChanged();
    void menuHidden();
    void onAddButtonClicked();
    void onDiscoverButtonClicked();
    void onRemoveButtonClicked();
    void rightClicked();
    void showContextMenu(QPoint pos);
    void showSystemDetails();
    void startTime(QTreeWidgetItem *item, int column);
    void validate(QTreeWidgetItem *, int column);

signals:
    void refreshProgress(int progress, String message = "");
    /**
     * @brief Emitted when host is removed from list
     * @param id -- ip or domain name of host
     */
    void removed(String id);
    void selectionChanged();

public:
    /**
     * @brief Constructor
     * @param parent -- parent of widget
     */
    explicit PCTreeWidget(QWidget *parent = 0);
    /**
     * @brief Destructor
     */
    ~PCTreeWidget();
    int getTimeSec() const;
    /**
     * @brief Getter
     * @return tree widget (list of hosts)
     */
    QTreeWidget *getTree();
    /**
     * @brief Find or create parent node of hosts
     * @param item_name -- parent node name
     * @return parent node
     */
    QTreeWidgetItem *topLevelNode(String item_name);
    /**
     * @brief Connect buttons' signals with handlers
     * @param toolbar -- contains buttons
     */
    void connectButtons(QToolBar *toolbar);
    void setComputerIcon(QIcon icon);
    /**
     * @brief Setter
     * @param state -- true if items can be edited, else false
     */
    void setTimeSec(int time_sec);
};

#endif // PCTREEWIDGET_H
