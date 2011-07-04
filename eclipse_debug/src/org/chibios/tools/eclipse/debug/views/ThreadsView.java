package org.chibios.tools.eclipse.debug.views;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map.Entry;
import java.util.Set;

import org.chibios.tools.eclipse.debug.utils.DebugProxy;
import org.chibios.tools.eclipse.debug.utils.DebugProxyException;
import org.chibios.tools.eclipse.debug.utils.HexUtils;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.*;
import org.eclipse.jface.viewers.*;
import org.eclipse.jface.action.*;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.ui.*;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;

import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.IDebugEventSetListener;
import org.eclipse.cdt.debug.internal.core.model.CDebugTarget;
import org.eclipse.wb.swt.SWTResourceManager;
import org.eclipse.wb.swt.ResourceManager;

/**
 * This sample class demonstrates how to plug-in a new workbench view. The view
 * shows data obtained from the model. The sample creates a dummy model on the
 * fly, but a real implementation would connect to the model available either in
 * this or another plug-in (e.g. the workspace). The view is connected to the
 * model using a content provider.
 * <p>
 * The view uses a label provider to define how model objects should be
 * presented in the view. Each view can present the same model objects using
 * different labels and icons, if needed. Alternatively, a single label provider
 * can be shared between views in order to ensure that objects of the same type
 * are presented in the same way everywhere.
 * </p>
 */

@SuppressWarnings("restriction")
public class ThreadsView extends ViewPart implements IDebugEventSetListener {

  /**
   * The ID of the view as specified by the extension.
   */
  public static final String ID = "org.chibios.tools.eclipse.debug.views.ThreadsView";

  private TableViewer viewer;
  private Action refreshAction;
  private Action doubleClickAction;

  private DebugProxy debugger;

  /**
   * The constructor.
   */
  public ThreadsView() {

  }

  /**
   * This is a callback that will allow us to create the viewer and initialize
   * it.
   */
  public void createPartControl(Composite parent) {
    viewer = new TableViewer(parent, SWT.MULTI | SWT.H_SCROLL | SWT.V_SCROLL);
    Table table = viewer.getTable();
    table.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    table.setHeaderVisible(true);

    TableColumn tblclmnAddress = new TableColumn(table, SWT.CENTER);
    tblclmnAddress.setWidth(72);
    tblclmnAddress.setText("Address");

    TableColumn tblclmnStack = new TableColumn(table, SWT.CENTER);
    tblclmnStack.setWidth(72);
    tblclmnStack.setText("Stack");

    TableColumn tblclmnName = new TableColumn(table, SWT.LEFT);
    tblclmnName.setWidth(144);
    tblclmnName.setText("Name");

    TableColumn tblclmnState = new TableColumn(table, SWT.RIGHT);
    tblclmnState.setWidth(72);
    tblclmnState.setText("State");

    TableColumn tblclmnFlags = new TableColumn(table, SWT.RIGHT);
    tblclmnFlags.setWidth(40);
    tblclmnFlags.setText("Flgs");

    TableColumn tblclmnPriority = new TableColumn(table, SWT.RIGHT);
    tblclmnPriority.setWidth(40);
    tblclmnPriority.setText("Prio");

    TableColumn tblclmnRefs = new TableColumn(table, SWT.RIGHT);
    tblclmnRefs.setWidth(40);
    tblclmnRefs.setText("Refs");

    TableColumn tblclmnTime = new TableColumn(table, SWT.RIGHT);
    tblclmnTime.setWidth(64);
    tblclmnTime.setText("Time");

    TableColumn tblclmnShared = new TableColumn(table, SWT.CENTER);
    tblclmnShared.setWidth(72);
    tblclmnShared.setText("Obj/Msg");

    // Create the help context id for the viewer's control
    PlatformUI.getWorkbench()
              .getHelpSystem()
              .setHelp(viewer.getControl(),
                       "org.chibios.tools.eclipse.debug.viewer");
    makeActions();
    hookContextMenu();
    hookDoubleClickAction();
    contributeToActionBars();

    DebugPlugin.getDefault().addDebugEventListener(this);

    try {
      debugger = new DebugProxy();
    } catch (DebugProxyException e) {}
  }

  @Override
  public void dispose() {
    DebugPlugin.getDefault().removeDebugEventListener(this);
  }

  /**
   * @brief Handling events from the debugger.
   */
  @Override
  public void handleDebugEvents(DebugEvent[] events) {
    for (DebugEvent event : events) {
      switch (event.getKind()) {
      case DebugEvent.CREATE:
        Object source = event.getSource();
        if (source instanceof CDebugTarget) {
          try {
            debugger = new DebugProxy((CDebugTarget)source);
          } catch (DebugProxyException e) {}
        }
        break;
      }
    }
  }

  private void hookContextMenu() {
    MenuManager menuMgr = new MenuManager("#PopupMenu");
    menuMgr.setRemoveAllWhenShown(true);
    menuMgr.addMenuListener(new IMenuListener() {
      public void menuAboutToShow(IMenuManager manager) {
        ThreadsView.this.fillContextMenu(manager);
      }
    });
    Menu menu = menuMgr.createContextMenu(viewer.getControl());
    viewer.getControl().setMenu(menu);
    getSite().registerContextMenu(menuMgr, viewer);
  }

  private void contributeToActionBars() {
    IActionBars bars = getViewSite().getActionBars();
    fillLocalPullDown(bars.getMenuManager());
    fillLocalToolBar(bars.getToolBarManager());
  }

  private void fillLocalPullDown(IMenuManager manager) {
    manager.add(refreshAction);
    manager.add(new Separator());
    manager.add(refreshAction);
  }

  private void fillContextMenu(IMenuManager manager) {
    manager.add(refreshAction);
    manager.add(refreshAction);
    // Other plug-ins can contribute there actions here
    manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
  }

  private void fillLocalToolBar(IToolBarManager manager) {
    manager.add(refreshAction);
  }

  private void makeActions() {
    
    // Threads table refresh action.
    refreshAction = new Action() {
      public void run() {
        LinkedHashMap<String, HashMap<String, String>> lhm;

        // If the debugger is not yet present then do nothing.
        if (debugger == null)
          return;

        // Reading the list of threads, null can be returned if the debugger
        // does not respond.
        try {
          lhm = debugger.readThreads();
          if (lhm == null)
            return;
        } catch (DebugProxyException e) {
          showMessage("Error: " + e.getMessage() + ".");
          return;
        }

        Table table = viewer.getTable();
        table.removeAll();

        Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
        for (Entry<String, HashMap<String, String>> entry : set) {
          HashMap<String, String> map = entry.getValue();
          TableItem tableItem = new TableItem(table, SWT.NONE);
          tableItem.setText(new String[] {
            HexUtils.dword2HexString(HexUtils.parseInt(entry.getKey())),
            HexUtils.dword2HexString(HexUtils.parseInt(map.get("stack"))),
            map.get("name"),
            map.get("state_s"),
            HexUtils.byte2HexString(HexUtils.parseInt(map.get("flags"))),
            map.get("prio"),
            map.get("refs"),
            map.get("time"),
            HexUtils.dword2HexString(HexUtils.parseInt(map.get("u")))
          });          
        }
      }
    };
    refreshAction.setDisabledImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/dlcl16/refresh_nav.gif"));
    refreshAction.setText("Refresh");
    refreshAction.setToolTipText("Refresh threads list");
    refreshAction.setImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/elcl16/refresh_nav.gif"));

    doubleClickAction = new Action() {
      public void run() {
        ISelection selection = viewer.getSelection();
        Object obj = ((IStructuredSelection) selection).getFirstElement();
        showMessage("Double-click detected on " + obj.toString());
      }
    };
  }

  private void hookDoubleClickAction() {
    viewer.addDoubleClickListener(new IDoubleClickListener() {
      public void doubleClick(DoubleClickEvent event) {
        doubleClickAction.run();
      }
    });
  }

  private void showMessage(String message) {
    MessageDialog.openInformation(viewer.getControl().getShell(),
                                  "ChibiOS/RT Threads View", message);
  }

  /**
   * Passing the focus request to the viewer's control.
   */
  public void setFocus() {
    viewer.getControl().setFocus();
  }
}
