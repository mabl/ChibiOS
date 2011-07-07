package org.chibios.tools.eclipse.debug.views;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.*;
import org.eclipse.jface.action.*;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.ui.*;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Table;
import org.eclipse.wb.swt.SWTResourceManager;

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

public class ChibiView extends ViewPart {

  /**
   * The ID of the view as specified by the extension.
   */
  public static final String ID = "org.chibios.tools.eclipse.debug.views.ChibiView";

  private CTabFolder tabFolder;

  private Action action1;
  private Action action2;
  private Table threadsTable;
  private Table timersTable;

  /**
   * The constructor.
   */
  public ChibiView() {
  }

  /**
   * This is a callback that will allow us to create the viewer and initialize
   * it.
   */
  public void createPartControl(Composite parent) {

    tabFolder = new CTabFolder(parent, SWT.BORDER);
    tabFolder.setSimple(false);
    tabFolder.setSelectionBackground(Display.getCurrent().getSystemColor(
        SWT.COLOR_TITLE_INACTIVE_BACKGROUND_GRADIENT));

    CTabItem tbtmGlobal = new CTabItem(tabFolder, SWT.NONE);
    tbtmGlobal.setText("Global");

    CTabItem tbtmThreads = new CTabItem(tabFolder, SWT.NONE);
    tbtmThreads.setText("Threads");

    threadsTable = new Table(tabFolder, SWT.BORDER | SWT.FULL_SELECTION);
    tbtmThreads.setControl(threadsTable);
    threadsTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    threadsTable.setHeaderVisible(true);

    CTabItem tbtmTimers = new CTabItem(tabFolder, SWT.NONE);
    tbtmTimers.setText("Timers");

    timersTable = new Table(tabFolder, SWT.BORDER | SWT.FULL_SELECTION);
    tbtmTimers.setControl(timersTable);
    timersTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    timersTable.setHeaderVisible(true);

    makeActions();
    hookContextMenu();
    hookDoubleClickAction();
    contributeToActionBars();
  }

  private void hookContextMenu() {
    MenuManager menuMgr = new MenuManager("#PopupMenu");
    menuMgr.setRemoveAllWhenShown(true);
    menuMgr.addMenuListener(new IMenuListener() {
      public void menuAboutToShow(IMenuManager manager) {
        ChibiView.this.fillContextMenu(manager);
      }
    });
  }

  private void contributeToActionBars() {
    IActionBars bars = getViewSite().getActionBars();
    fillLocalPullDown(bars.getMenuManager());
    fillLocalToolBar(bars.getToolBarManager());
  }

  private void fillLocalPullDown(IMenuManager manager) {
    manager.add(action1);
    manager.add(new Separator());
    manager.add(action2);
  }

  private void fillContextMenu(IMenuManager manager) {
    manager.add(action1);
    manager.add(action2);
    // Other plug-ins can contribute there actions here
    manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
  }

  private void fillLocalToolBar(IToolBarManager manager) {
    manager.add(action1);
    manager.add(action2);
  }

  private void makeActions() {
    action1 = new Action() {
      public void run() {
        showMessage("Action 1 executed");
      }
    };
    action1.setText("Action 1");
    action1.setToolTipText("Action 1 tooltip");
    action1.setImageDescriptor(PlatformUI.getWorkbench().getSharedImages()
        .getImageDescriptor(ISharedImages.IMG_OBJS_INFO_TSK));

    action2 = new Action() {
      public void run() {
        showMessage("Action 2 executed");
      }
    };
    action2.setText("Action 2");
    action2.setToolTipText("Action 2 tooltip");
    action2.setImageDescriptor(PlatformUI.getWorkbench().getSharedImages()
        .getImageDescriptor(ISharedImages.IMG_OBJS_INFO_TSK));
/*    doubleClickAction = new Action() {
      public void run() {
        ISelection selection = viewer.getSelection();
        Object obj = ((IStructuredSelection) selection).getFirstElement();
        showMessage("Double-click detected on " + obj.toString());
      }
    };*/
  }

  private void hookDoubleClickAction() {
  }

  private void showMessage(String message) {
    MessageDialog.openInformation(tabFolder.getShell(),
        "ChibiOS/RT Views", message);
  }

  /**
   * Passing the focus request to the viewer's control.
   */
  public void setFocus() {
    tabFolder.setFocus();
  }
}
