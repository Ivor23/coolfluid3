#ifndef CF_GUI_Client_TreeView_h
#define CF_GUI_Client_TreeView_h

////////////////////////////////////////////////////////////////////////////////

#include <QTreeView>
#include <QList>
#include <QHash>

#include "GUI/Network/ComponentType.hpp"

#include "GUI/Client/TSshInformation.hpp"

class QDomDocument;
class QDomNode;
class QMainWindow;
class QMenu;
class QMenuBar;
class QModelIndex;
class QSortFilterProxyModel;

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {
namespace Client {

////////////////////////////////////////////////////////////////////////////////

  class OptionPanel;
  class NodeAction;
  struct TSshInformation;

  /////////////////////////////////////////////////////////////////////////////

  /// @brief This class manages the tree m_view.

  /// @author Gasper Quentin

  class TreeView : public QTreeView
  {
    Q_OBJECT

  public:
    /// @brief Constructor.

    /// @param optionsPanel Panel m_options of the selected node will be displayed.
    /// @param parent Parent window. May be @c CFNULL.
    /// @throws std::invalid_argument if @c optionsPanel is @c CFNULL.
    TreeView(OptionPanel * optionsPanel, QMainWindow * parent = CFNULL);

    /// @brief Destructor.

    /// Frees all allocated memory. Neither the m_options panel nor the parent
    /// are destroyed.
    ~TreeView();

    /// @brief Builds a new node.

    /// This method calls @link ::TreeModel::newChildToNode()
    /// TreeModel::newChildToNode() @endlink for the currently selected index.
    /// @param newNode New node name.
    /// @param doc The document the node will be added to. The presence of
    /// this parameter is due to the fact that a node can not exist if it
    /// does not belong to a document.
    /// @return The built node.
    QDomNode newChildNode(const QString & newNode, QDomDocument & doc) const;

    /// @brief Changes the read-only mode.

    /// @param readOnly If @c true, the tree m_view will switch to the read-only
    /// mode.
    void setReadOnly(bool readOnly);

    /// @brief Indicates whether the tree m_view is in read-only mode or not.

    /// @return Returns @c true if the tree m_view is in read-only mode.
    /// Otherwise, returns @c false.
    bool isReadOnly() const;

  protected:

    /// @brief Method called when a mouse button is pressed in the Client.

    /// This method overloads parent class method. Four cases are possible :
    /// @li If user right-clicks, a context menu is displayed.
    /// @li If user left-clicks on another node than the currently selected one
    /// @b and @c confirmChangeOptions() returns @c true, m_options in the
    /// m_options panel are changed.
    /// @li If user left-clicks on the selected node nothing is done.
    /// @li Middle button has no effect.
    /// @param event Event that occured.
    virtual void mousePressEvent(QMouseEvent * event);

    virtual void keyPressEvent(QKeyEvent * event);

  private slots:

    /// @brief Slot called when user wants to update a simulation tree.
    void updateTree();

    void currentIndexChanged(const QModelIndex & newIndex, const QModelIndex & oldIndex);

    void nodeActivated(const QModelIndex & index);

  signals:

    void addLink(const QModelIndex & index, const QString & name,
                 const QModelIndex & target);

    /// @brief Signal emitted when user wants to add a node.

    /// @param abstractType Abstract type of the new node.
    void addNode(const QString & abstractType);

    /// @brief Signal emitted when user wants to delete a node.

    /// @param node Node index in the model.
    void deleteNode(const QDomNode & node);

    /// @brief Signal emitted when user wants to rename a node.

    /// @param node Node index in the model.
    /// @param newName Node new name.
    void renameNode(const QDomNode & node, const QString & newName);

    /// @brief Signal emitted when user wants to commit modified m_options

    /// @param document Modification information
    void commitChanges(const QDomDocument & document);

    /// @brief Signal emitted when user wants to connect a simulation to its
    /// server.

    /// @param index Simulation index
    /// @todo Replace QModelIndex by QPersistentModelIndex
    void connectSimulation(const QModelIndex & index,
                           const TSshInformation & info);

    /// @brief Signal emitted when user wants to connect a simulation to its
    /// server.

    /// @param index Simulation index
    /// @param shutServer If @c true, the server shuts down. Otherwise, it keeps
    /// running after the client has been disconnected.
    /// @todo Replace QModelIndex by QPersistentModelIndex
    void disconnectSimulation(const QModelIndex & index, bool shutServer);

    /// @brief Signal emitted when user wants to load a case file.

    /// @param index Simulation index
    /// @todo Replace QModelIndex by QPersistentModelIndex
    void openSimulation(const QModelIndex & index);

    /// @brief Signal emitted when user wants to run a simulation.

    /// @param index Simulation index
    /// @todo Replace QModelIndex by QPersistentModelIndex
    void runSimulation(const QModelIndex & index);

    /// @brief Signal emitted when user wants to stop a running simulation.

    /// @param index Simulation index
    /// @todo Replace QModelIndex by QPersistentModelIndex
    void stopSimulation(const QModelIndex & index);

    /// @brief Signal emitted when user wants to activate a simulation.

    /// @param index Simulation index
    /// @todo Replace QModelIndex by QPersistentModelIndex
    void activateSimulation(const QModelIndex & index);

    /// @brief Signal emitted when user wants to deactivcate a simulation.

    /// @param index Simulation index
    /// @todo Replace QModelIndex by QPersistentModelIndex
    void deactivateSimulation(const QModelIndex & index);

    void updateTree(const QModelIndex & index);

  private:

    /// @brief Panel used to display and modify m_options for a selected
    /// object.
    OptionPanel * m_optionsPanel;

    /// @brief List of abstract types
    QStringList m_abstractTypes;

    /// @brief Filter for the Client.

    /// Allows to switch between basic/advanced mode. The filter is used as the
    /// Client model. Its source is the tree model.
    QSortFilterProxyModel * m_modelFilter;

    /// @brief Indicates whether the tree m_view is in read-only mode or not.

    /// If @c true, the tree m_view is read-only mode. When it is read-only mode,
    /// all m_options in the context that may modify an object are disbaled.
    /// "Delete", "Rename", "Add a child node" and "Add an option" m_items are then
    /// disabled.
    bool m_readOnly;

    /// @brief Asks user to commit or rollback before changing m_options in
    /// m_options panel.

    /// If modifications were committed, nothing is asked and the method
    /// immediately returns @c true. If the commit is requested by the user,
    /// it is processed by this method.
    /// @param index Node index on which user clicked. If it is equals to
    /// @c #currentIndexInPanel nothing is asked and the method
    /// immediately returns @c true.
    /// @param okIfSameIndex If @c false, the method checks if indexes are the
    /// same. If @c true, no check is done.
    /// @return Returns @c false if the user clicked on "Cancel" ; otherwise
    /// returns @c true.
    bool confirmChangeOptions(const QModelIndex & index, bool okIfSameIndex = false);

  }; // class TreeView

  /////////////////////////////////////////////////////////////////////////////

} // namespace Client
} // namespace GUI
} // namespace CF

  /////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_TreeView_h
