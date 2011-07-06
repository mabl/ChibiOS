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
 * <p>
 */

@SuppressWarnings("restriction")
public class TimersView extends ViewPart implements IDebugEventSetListener {

  /**
   * The ID of the view as specified by the extension.
   */
  public static final String ID = "org.chibios.tools.eclipse.debug.views.TimersView";

  private TableViewer viewer;
  private Action refreshAction;
  private Action doubleClickAction;

  private DebugProxy debugger;

  /**
   * The constructor.
   */
  public TimersView() {
  }

  /**
   * This is a callback that will allow us to create the viewer and initialize
   * it.
   */
  public void createPartControl(Composite parent) {
    viewer = new TableViewer(parent, SWT.MULTI | SWT.H_SCROLL | SWT.V_SCROLL);
    Table table = viewer.getTable();
    table.setHeaderVisible(true);
    table.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));

    TableColumn tblclmnAddress = new TableColumn(table, SWT.CENTER);
    tblclmnAddress.setWidth(72);
    tblclmnAddress.setText("Address");

    TableColumn tblclmnTime = new TableColumn(table, SWT.RIGHT);
    tblclmnTime.setWidth(72);
    tblclmnTime.setText("Time");

    TableColumn tblclmnDelta = new TableColumn(table, SWT.RIGHT);
    tblclmnDelta.setWidth(72);
    tblclmnDelta.setText("Delta");

    TableColumn tblclmnCallback = new TableColumn(table, SWT.CENTER);
    tblclmnCallback.setWidth(72);
    tblclmnCallback.setText("Callback");

    TableColumn tblclmnParameter = new TableColumn(table, SWT.CENTER);
    tblclmnParameter.setWidth(72);
    tblclmnParameter.setText("Param");

    // Create the help context id for the viewer's control
    PlatformUI.getWorkbench().getHelpSystem()
        .setHelp(viewer.getControl(), "org.chibios.tools.eclipse.debug.viewer");
    makeActions();
    hookContextMenu();
    hookDoubleClickAction();
    contributeToActionBars();

    DebugPlugin.getDefault().addDebugEventListener(this);

    try {
      debugger = new DebugProxy();
    } catch (DebugProxyException e) {}
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
        TimersView.this.fillContextMenu(manager);
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
    
    // Timers table refresh action.
    refreshAction = new Action() {
      public void run() {
        LinkedHashMap<String, HashMap<String, String>> lhm;

        // If the debugger is not yet present then do nothing.
        if (debugger == null)
          return;

        // Reading the list of threads, null can be returned if the debugger
        // does not respond.
        try {
          lhm = debugger.readTimers();
          if (lhm == null)
            return;
        } catch (DebugProxyException e) {
          showMessage("Error: " + e.getMessage() + ".");
          return;
        }

        Table table = viewer.getTable();
        table.removeAll();

        Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
        long time = 0;
        for (Entry<String, HashMap<String, String>> entry : set) {
          HashMap<String, String> map = entry.getValue();
          time = time + HexUtils.parseInt(map.get("delta"));
          TableItem tableItem = new TableItem(table, SWT.NONE);
          tableItem.setText(new String[] {
            HexUtils.dword2HexString(HexUtils.parseInt(entry.getKey())),
            Long.toString(time),
            "+" + HexUtils.parseInt(map.get("delta")),
            HexUtils.dword2HexString(HexUtils.parseInt(map.get("func"))),
            HexUtils.dword2HexString(HexUtils.parseInt(map.get("par")))
          });          
        }
      }
    };
    refreshAction.setDisabledImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/dlcl16/refresh_nav.gif"));
    refreshAction.setImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/elcl16/refresh_nav.gif"));
    refreshAction.setText("Refresh");
    refreshAction.setToolTipText("Refresh timers list");

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
                                  "ChibiOS/RT Timers View", message);
  }

  /**
   * Passing the focus request to the viewer's control.
   */
  public void setFocus() {
    viewer.getControl().setFocus();
  }
}
