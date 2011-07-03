package org.chibios.tools.eclipse.debug.views;

import java.util.HashMap;
import java.util.LinkedHashMap;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.*;
import org.eclipse.jface.viewers.*;
import org.eclipse.swt.graphics.Image;
import org.eclipse.jface.action.*;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.ui.*;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;

import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.IDebugEventSetListener;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.ui.DebugUITools;
import org.eclipse.cdt.debug.core.cdi.ICDISession;
import org.eclipse.cdt.debug.core.cdi.model.ICDITarget;
import org.eclipse.cdt.debug.internal.core.model.CDebugTarget;
import org.eclipse.cdt.debug.mi.core.cdi.model.Target;
import org.eclipse.cdt.debug.mi.core.command.CommandFactory;
import org.eclipse.cdt.debug.mi.core.command.MIDataEvaluateExpression;
import org.eclipse.cdt.debug.mi.core.command.MIDataReadMemory;
import org.eclipse.cdt.debug.mi.core.output.MIDataEvaluateExpressionInfo;
import org.eclipse.cdt.debug.mi.core.output.MIDataReadMemoryInfo;
import org.eclipse.cdt.debug.mi.core.MIFormat;
import org.eclipse.cdt.debug.mi.core.MISession;
import org.eclipse.cdt.debug.mi.core.MIException;
import org.eclipse.cdt.debug.mi.core.output.MIMemory;

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
    
  private MISession miSession;

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
    
    TableItem tableItem = new TableItem(table, SWT.NONE);
    tableItem.setText(new String[] {"One", "Two", "Three"});

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
  }

  @Override public void dispose() {

    DebugPlugin.getDefault().removeDebugEventListener(this);
  }

/*  private void getDebugger() {
    IDebugTarget[] debug_targets = DebugPlugin.getDefault().getLaunchManager().getDebugTargets();
    for (IDebugTarget target:debug_targets) {
      if(target instanceof CDebugTarget) {
        CDebugTarget debug_target = (CDebugTarget)target;
        ICDISession cdi_session = debug_target.getCDISession();
        ICDITarget[] targets = cdi_session.getTargets();
        miSession = null;
        ICDITarget cdi_target = null;
        for (int i = 0; i < targets.length; i++) {
          if (targets[i] instanceof Target) {
            cdi_target = targets[i];
            break;
          }
        }
        if (cdi_target != null) {
          miSession = ((Target)cdi_target).getMISession();
        }
      }
      if(miSession != null)
        return;
    }
  }

  private Object[] getThreads() {
    
    return null;
  }*/

  private String evaluateExpression(CommandFactory factory, String expression) {
    
    MIDataEvaluateExpression expr = factory.createMIDataEvaluateExpression(expression);
    try {
      miSession.postCommand(expr);
      MIDataEvaluateExpressionInfo info = expr.getMIDataEvaluateExpressionInfo();
      if (info != null)
        return info.getExpression();
    } catch (MIException e) {
      return null;
    }
    return null;
  }

  private void readThreads() {
    LinkedHashMap lhm = new LinkedHashMap<String, HashMap>(10);
    
    if (miSession != null) {
      CommandFactory factory = miSession.getCommandFactory();
      // Scanning Registry linked list
      String rlist = evaluateExpression(factory, "&rlist");
      String newer = evaluateExpression(factory, "rlist.r_newer");

    }
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
          CDebugTarget debug_target = (CDebugTarget)source;
          ICDISession cdi_session = debug_target.getCDISession();
          ICDITarget[] targets = cdi_session.getTargets();
          miSession = null;
          ICDITarget cdi_target = null;
          for (int i = 0; i < targets.length; i++) {
            if (targets[i] instanceof Target) {
              cdi_target = targets[i];
              break;
            }
          }
          if (cdi_target != null) {
            miSession = ((Target)cdi_target).getMISession();
          }
        }
        break;
      case DebugEvent.TERMINATE:
        miSession = null;
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
//        readThreads();
        CommandFactory factory = miSession.getCommandFactory();
        String s = evaluateExpression(factory, "rlist");
        if (s == null)
          showMessage("error");
        else
          showMessage(s);
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
