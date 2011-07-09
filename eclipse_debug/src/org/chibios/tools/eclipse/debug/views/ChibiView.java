package org.chibios.tools.eclipse.debug.views;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map.Entry;
import java.util.Set;

import org.chibios.tools.eclipse.debug.utils.DebugProxy;
import org.chibios.tools.eclipse.debug.utils.DebugProxyException;
import org.chibios.tools.eclipse.debug.utils.HexUtils;

import org.eclipse.ui.part.*;
import org.eclipse.jface.action.*;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.ui.*;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.widgets.Composite;
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
public class ChibiView extends ViewPart implements IDebugEventSetListener {

  /**
   * The ID of the view as specified by the extension.
   */
  public static final String ID = "org.chibios.tools.eclipse.debug.views.ChibiView";

  private CTabFolder tabFolder;
  private CTabItem tbtmGlobal;
  private CTabItem tbtmThreads;
  private CTabItem tbtmTimers;
  private CTabItem tbtmTraceBuffer;

  private Action refreshAction;
  private Table threadsTable;
  private Table timersTable;

  private DebugProxy debugger;
  private Table tbTable;

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

    tabFolder = new CTabFolder(parent, SWT.BORDER | SWT.FLAT | SWT.BOTTOM);
    tabFolder.setSelectionBackground(SWTResourceManager.getColor(SWT.COLOR_WIDGET_LIGHT_SHADOW));
    tabFolder.setSelectionForeground(SWTResourceManager.getColor(0, 0, 0));
    tabFolder.setSimple(false);

    tbtmGlobal = new CTabItem(tabFolder, SWT.NONE);
    tbtmGlobal.setText("Global");

    tbtmThreads = new CTabItem(tabFolder, SWT.NONE);
    tbtmThreads.setText("Threads");

    threadsTable = new Table(tabFolder, SWT.FULL_SELECTION);
    tbtmThreads.setControl(threadsTable);
    threadsTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    threadsTable.setHeaderVisible(true);

    TableColumn tblclmnThreadAddress = new TableColumn(threadsTable, SWT.CENTER);
    tblclmnThreadAddress.setWidth(72);
    tblclmnThreadAddress.setText("Address");

    TableColumn tblclmnThreadStack = new TableColumn(threadsTable, SWT.CENTER);
    tblclmnThreadStack.setWidth(72);
    tblclmnThreadStack.setText("Stack");

    TableColumn tblclmnThreadName = new TableColumn(threadsTable, SWT.LEFT);
    tblclmnThreadName.setWidth(144);
    tblclmnThreadName.setText("Name");

    TableColumn tblclmnThreadState = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadState.setWidth(72);
    tblclmnThreadState.setText("State");

    TableColumn tblclmnThreadFlags = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadFlags.setWidth(40);
    tblclmnThreadFlags.setText("Flgs");

    TableColumn tblclmnThreadPriority = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadPriority.setWidth(40);
    tblclmnThreadPriority.setText("Prio");

    TableColumn tblclmnThreadRefs = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadRefs.setWidth(40);
    tblclmnThreadRefs.setText("Refs");

    TableColumn tblclmnThreadTime = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadTime.setWidth(64);
    tblclmnThreadTime.setText("Time");

    TableColumn tblclmnThreadShared = new TableColumn(threadsTable, SWT.CENTER);
    tblclmnThreadShared.setWidth(72);
    tblclmnThreadShared.setText("Obj/Msg");

    tbtmTimers = new CTabItem(tabFolder, SWT.NONE);
    tbtmTimers.setText("Timers");

    timersTable = new Table(tabFolder, SWT.FULL_SELECTION);
    tbtmTimers.setControl(timersTable);
    timersTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    timersTable.setHeaderVisible(true);

    TableColumn tblclmnTimerAddress = new TableColumn(timersTable, SWT.CENTER);
    tblclmnTimerAddress.setWidth(72);
    tblclmnTimerAddress.setText("Address");

    TableColumn tblclmnTimerTime = new TableColumn(timersTable, SWT.RIGHT);
    tblclmnTimerTime.setWidth(72);
    tblclmnTimerTime.setText("Time");

    TableColumn tblclmnTimerDelta = new TableColumn(timersTable, SWT.RIGHT);
    tblclmnTimerDelta.setWidth(72);
    tblclmnTimerDelta.setText("Delta");

    TableColumn tblclmnTimerCallback = new TableColumn(timersTable, SWT.CENTER);
    tblclmnTimerCallback.setWidth(72);
    tblclmnTimerCallback.setText("Callback");

    TableColumn tblclmnTimerParameter = new TableColumn(timersTable, SWT.CENTER);
    tblclmnTimerParameter.setWidth(72);
    tblclmnTimerParameter.setText("Param");
    
    tbtmTraceBuffer = new CTabItem(tabFolder, SWT.NONE);
    tbtmTraceBuffer.setText("TraceBuffer");
    
    tbTable = new Table(tabFolder, SWT.BORDER | SWT.FULL_SELECTION);
    tbTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    tbtmTraceBuffer.setControl(tbTable);
    tbTable.setHeaderVisible(true);

    TableColumn tblclmnTraceBufferHidden = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferHidden.setWidth(0);
    tblclmnTraceBufferHidden.setText("");

    TableColumn tblclmnTraceBufferIndex = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferIndex.setWidth(48);
    tblclmnTraceBufferIndex.setText("Event");

    TableColumn tblclmnTraceBufferTime = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferTime.setWidth(64);
    tblclmnTraceBufferTime.setText("Time");

    TableColumn tblclmnTraceBufferPrevAddress = new TableColumn(tbTable, SWT.CENTER);
    tblclmnTraceBufferPrevAddress.setWidth(72);
    tblclmnTraceBufferPrevAddress.setText("Previous");

    TableColumn tblclmnTraceBufferPrevName = new TableColumn(tbTable, SWT.LEFT);
    tblclmnTraceBufferPrevName.setWidth(144);
    tblclmnTraceBufferPrevName.setText("Previous Name");

    TableColumn tblclmnTraceBufferState = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferState.setWidth(72);
    tblclmnTraceBufferState.setText("State");

    TableColumn tblclmnTraceBufferShared = new TableColumn(tbTable, SWT.CENTER);
    tblclmnTraceBufferShared.setWidth(72);
    tblclmnTraceBufferShared.setText("Obj/Msg");

    TableColumn tblclmnTraceBufferCurrentAddress = new TableColumn(tbTable, SWT.CENTER);
    tblclmnTraceBufferCurrentAddress.setWidth(72);
    tblclmnTraceBufferCurrentAddress.setText("Current");

    TableColumn tblclmnTraceBufferCurrentName = new TableColumn(tbTable, SWT.LEFT);
    tblclmnTraceBufferCurrentName.setWidth(144);
    tblclmnTraceBufferCurrentName.setText("Current Name");

    makeActions();
    hookContextMenu();
    contributeToActionBars();

    tabFolder.setSelection(tbtmGlobal);

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
    manager.add(refreshAction);
    manager.add(new Separator());
    manager.add(refreshAction);
  }

  private void fillContextMenu(IMenuManager manager) {
    manager.add(refreshAction);
    // Other plug-ins can contribute there actions here
    manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
  }

  private void fillLocalToolBar(IToolBarManager manager) {
    manager.add(refreshAction);
  }

  private void fillThreadsTable() {
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

    threadsTable.removeAll();

    Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
    for (Entry<String, HashMap<String, String>> entry : set) {
      HashMap<String, String> map = entry.getValue();
      TableItem tableItem = new TableItem(threadsTable, SWT.NONE);
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

  private void fillTimersTable() {
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

    timersTable.removeAll();

    Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
    long time = 0;
    for (Entry<String, HashMap<String, String>> entry : set) {
      HashMap<String, String> map = entry.getValue();
      time = time + HexUtils.parseInt(map.get("delta"));
      TableItem tableItem = new TableItem(timersTable, SWT.NONE);
      tableItem.setText(new String[] {
        HexUtils.dword2HexString(HexUtils.parseInt(entry.getKey())),
        Long.toString(time),
        "+" + HexUtils.parseInt(map.get("delta")),
        HexUtils.dword2HexString(HexUtils.parseInt(map.get("func"))),
        HexUtils.dword2HexString(HexUtils.parseInt(map.get("par")))
      });          
    }
  }

  private void fillTraceBufferTable() {
    LinkedHashMap<String, HashMap<String, String>> lhm;

    // If the debugger is not yet present then do nothing.
    if (debugger == null)
      return;

    // Reading the list of threads, null can be returned if the debugger
    // does not respond.
    try {
      lhm = debugger.readTraceBuffer();
      if (lhm == null)
        return;
    } catch (DebugProxyException e) {
      showMessage("Error: " + e.getMessage() + ".");
      return;
    }

    tbTable.removeAll();

    Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
    String prev = "";
    String prevname = "";
    for (Entry<String, HashMap<String, String>> entry : set) {
      HashMap<String, String> map = entry.getValue();
      TableItem tableItem = new TableItem(tbTable, SWT.NONE);
      String current = HexUtils.dword2HexString(HexUtils.parseInt(map.get("tp")));
      String currentname = "name";
      tableItem.setText(new String[] {
        "",
        entry.getKey(),
        map.get("time"),
        prev,
        prevname,
        map.get("state_s"),
        HexUtils.dword2HexString(HexUtils.parseInt(map.get("wtobjp"))),
        current,
        currentname
      });
      prev = current;
      prevname = currentname;
    }
  }

  private void makeActions() {
    
    // Refresh action.
    refreshAction = new Action() {
      public void run() {
        CTabItem tabitem = tabFolder.getSelection();
        if (tabitem == null)
          return;
        if (tabitem == tbtmGlobal)
          ;
        else if (tabitem == tbtmThreads)
          fillThreadsTable();
        else if (tabitem == tbtmTimers)
          fillTimersTable();
        else if (tabitem == tbtmTraceBuffer)
          fillTraceBufferTable();
      }
    };
    refreshAction.setDisabledImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/dlcl16/refresh_nav.gif"));
    refreshAction.setImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/elcl16/refresh_nav.gif"));
    refreshAction.setText("Refresh");
    refreshAction.setToolTipText("Refresh timers list");
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
