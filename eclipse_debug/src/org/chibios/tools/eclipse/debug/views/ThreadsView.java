package org.chibios.tools.eclipse.debug.views;

import java.awt.RenderingHints.Key;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map.Entry;
import java.util.Set;

import org.chibios.tools.debug.utils.DebugProxy;
import org.chibios.tools.debug.utils.DebugProxyException;
import org.chibios.tools.debug.utils.HexUtils;
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

public class ThreadsView extends ViewPart implements IDebugEventSetListener {

  /**
   * The ID of the view as specified by the extension.
   */
  public static final String ID = "org.chibios.tools.eclipse.debug.views.ThreadsView";

  private TableViewer viewer;
  private Action action1;
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
    table.setHeaderVisible(true);
    
    TableColumn tblclmnNewColumn = new TableColumn(table, SWT.NONE);
    tblclmnNewColumn.setWidth(100);
    tblclmnNewColumn.setText("Address");
    
    TableColumn tblclmnName = new TableColumn(table, SWT.NONE);
    tblclmnName.setWidth(100);
    tblclmnName.setText("Name");
    
    TableColumn tblclmnPriority = new TableColumn(table, SWT.NONE);
    tblclmnPriority.setWidth(48);
    tblclmnPriority.setText("Priority");

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

  @Override public void dispose() {

    DebugPlugin.getDefault().removeDebugEventListener(this);
  }

  private LinkedHashMap<String, HashMap<String, String>> readThreads() {
    LinkedHashMap<String, HashMap<String, String>> lhm;
    
    lhm = new LinkedHashMap<String, HashMap<String, String>>(10);
    if (debugger != null) {
      // Scanning Registry linked list
      try {
        // rlist structure address and first thread in the registry.
        String rlist = debugger.evaluateExpression("&rlist");
        String newer = debugger.evaluateExpression("rlist.r_newer");
        
        // This can happen if the kernel is not initialized yet.
        if (newer.compareTo("0x0") == 0)
          return lhm;
        
        // Scanning registry.
        while (newer.compareTo(rlist) != 0) {
          // Hash of threads fields.
          HashMap<String, String> map = new HashMap<String, String>(16);
          
          // Fetch of the various fields in the Thread structure.
          int prio = HexUtils.parseInt(debugger.evaluateExpression("((Thread *)" + newer + ")->p_prio"));
          
          // Populating map of the thread fields.
          map.put("prio", Integer.toString(prio));
          
          // Inserting the new thread map into the threads list.
          lhm.put(newer, map);

          // Next thread in the registry, probably sanity checks should be
          // improved.
          newer = debugger.evaluateExpression("((Thread *)" + newer + ")->p_newer");
          if (newer.compareTo("0x0") == 0)
            return lhm;
        }
      } catch (DebugProxyException e) {
      }
    }
    return lhm;
  }

  /**
   * @brief Handling events from the debugger.
   */
  @Override public void handleDebugEvents(DebugEvent[] events) {

    for (DebugEvent event : events) {
      switch (event.getKind()) {
      case DebugEvent.SUSPEND:
        Object source = event.getSource();
        if (source instanceof CDebugTarget) {
          try {
            debugger = new DebugProxy((CDebugTarget)source);
          } catch (DebugProxyException e) {}
        }
        break;
      case DebugEvent.TERMINATE:
        debugger = null;
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
    manager.add(action1);
    manager.add(new Separator());
    manager.add(action1);
  }
   
  private void fillContextMenu(IMenuManager manager) {
    manager.add(action1);
    manager.add(action1);
    // Other plug-ins can contribute there actions here
    manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
  }

  private void fillLocalToolBar(IToolBarManager manager) {
    manager.add(action1);
  }

  private void makeActions() {
    action1 = new Action() {
      public void run() {
        LinkedHashMap<String, HashMap<String, String>> lhm = readThreads();

        Table table = viewer.getTable();
        table.removeAll();
        
        Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
        for (Entry<String, HashMap<String, String>> entry : set) {
          HashMap<String, String> map = entry.getValue();
          TableItem tableItem = new TableItem(table, SWT.NONE);
          tableItem.setText(new String[] {
            HexUtils.dword2HexString(HexUtils.parseInt(entry.getKey())),
            "",
            map.get("prio")
          });          
        }

/*        try {
          s = debugger.evaluateExpression("rlist");
        } catch (DebugProxyException e) {
          showMessage(e.toString());
          return;
        }
        showMessage(s);*/
      }
    };
    action1.setText("Action 1");
    action1.setToolTipText("Action 1 tooltip");
    action1.setImageDescriptor(PlatformUI.getWorkbench()
                                         .getSharedImages()
                                         .getImageDescriptor(ISharedImages.IMG_ELCL_SYNCED));

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
