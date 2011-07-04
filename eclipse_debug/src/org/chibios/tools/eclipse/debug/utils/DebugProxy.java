package org.chibios.tools.eclipse.debug.utils;

import java.util.HashMap;
import java.util.LinkedHashMap;

import org.eclipse.cdt.debug.core.cdi.model.ICDITarget;
import org.eclipse.cdt.debug.internal.core.model.CDebugTarget;
import org.eclipse.cdt.debug.mi.core.MIException;
import org.eclipse.cdt.debug.mi.core.MISession;
import org.eclipse.cdt.debug.mi.core.cdi.model.Target;
import org.eclipse.cdt.debug.mi.core.command.CommandFactory;
import org.eclipse.cdt.debug.mi.core.command.MIDataEvaluateExpression;
import org.eclipse.cdt.debug.mi.core.output.MIDataEvaluateExpressionInfo;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.model.IDebugTarget;

public class DebugProxy {

  private CommandFactory cmd_factory;
  private MISession mi_session;

  protected final static String[] threadStates = {
    "READY",
    "CURRENT",
    "SUSPENDED",
    "WTSEM",
    "WTMTX",
    "WTCOND",
    "SLEEPING",
    "WTEXIT",
    "WTOREVT",
    "WTANDEVT",
    "SNDMSGQ",
    "SNDMSG",
    "WTMSG",
    "WTQUEUE",
    "FINAL"
  };

  private void getSession(CDebugTarget target)
      throws DebugProxyException {
    
    ICDITarget[] targets = target.getCDISession().getTargets();
    ICDITarget cdi_target = null;
    for (int i = 0; i < targets.length; i++) {
      if (targets[i] instanceof Target) {
        cdi_target = targets[i];
        break;
      }
    }
    if (cdi_target == null)
      throw new DebugProxyException("no CDI session found");
    mi_session = ((Target)cdi_target).getMISession();
    cmd_factory = mi_session.getCommandFactory();
  }

  public DebugProxy()
      throws DebugProxyException {
    
    IDebugTarget[] targets = DebugPlugin.getDefault().getLaunchManager().getDebugTargets();
    for (IDebugTarget target:targets) {
      if(target instanceof CDebugTarget) {
        getSession((CDebugTarget)target);
        return;
      }
    }
  }

  public DebugProxy(CDebugTarget target)
      throws DebugProxyException {
    
    getSession(target);
  }

  public String evaluateExpression(String expression)
      throws DebugProxyException {
    
    MIDataEvaluateExpression expr = cmd_factory.createMIDataEvaluateExpression(expression);
    try {
      mi_session.postCommand(expr);
      MIDataEvaluateExpressionInfo info = expr.getMIDataEvaluateExpressionInfo();
      if (info != null)
        return info.getExpression();
    } catch (MIException e) {}
    throw new DebugProxyException("error evaluating the expression: '" +
                                  expression + "'");
  }

  /**
   * @brief   Return the list of threads.
   * @details The threads list is fetched from memory by scanning the
   *          registry.
   *
   * @return  A @p LinkedHashMap object whose keys are the threads addresses
   *          as decimal strings, the value is an @p HashMap of the thread
   *          fields:
   *          - stack
   *          - state
   *          - state_s
   *          - flags
   *          - prio
   *          - refs
   *          - time
   *          - u
   *          .
   *          Missing fields are set to "-".
   * @retval null                   If the debugger encountered an error.
   *
   * @throws DebugProxyException    If the debugger is active but the registry
   *                                is not found, not initialized or corrupted.
   */
  public LinkedHashMap<String, HashMap<String, String>> readThreads()
      throws DebugProxyException {

    // rlist structure address.
    String rlist;
    try {
      rlist = evaluateExpression("(uint32_t)&rlist");
    } catch (DebugProxyException e) {
      throw new DebugProxyException("ChibiOS/RT not found on target");
    } catch (Exception e) {
      return null;
    }

    // Scanning registry.
    LinkedHashMap<String, HashMap<String, String>> lhm =
        new LinkedHashMap<String, HashMap<String, String>>(10);
    String current = rlist;
    String previous = rlist;
    while (true) {
      
      // Fetching next thread in the registry (newer link). This fetch fails
      // if the register is not enabled in the kernel and the p_newer field
      // does not exist.
      try {
        current = evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_newer");
      } catch (DebugProxyException e1) {
        throw new DebugProxyException("ChibiOS/RT registry not enabled in kernel");
      }

      // This can happen if the kernel is not initialized yet or if the
      // registry is corrupted.
      if (current.compareTo("0") == 0)
        throw new DebugProxyException("ChibiOS/RT registry sanity check failed, NULL pointer");

      // TODO: Sanity check on the pointer value (alignment, range).

      // End of the linked list condition.
      if (current.compareTo(rlist) != 0)
        break;

      // The previous thread in the list is fetched as a sanity check.
      String older = evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_older");
      if (older.compareTo("0") == 0)
        throw new DebugProxyException("ChibiOS/RT registry sanity check failed, NULL pointer");
      if (previous.compareTo(older) != 0)
        throw new DebugProxyException("ChibiOS/RT registry sanity check failed, double linked list violation");

      // Hash of threads fields.
      HashMap<String, String> map = new HashMap<String, String>(16);

      // Fetch of the various fields in the Thread structure. Some fields
      // are optional so are placed within try-catch.
      int n;
      try {
        n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_ctx.r13"));
        map.put("stack", Integer.toString(n));
      } catch (DebugProxyException e) {
        try {
          n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_ctx.sp"));
          map.put("stack", Integer.toString(n));
        } catch (DebugProxyException ex) {
          map.put("stack", "-");
        }
      }

      n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_state"));
      map.put("state", Integer.toString(n));
      if ((n >= 0) && (n < threadStates.length)) {
        map.put("state_s", threadStates[n]);
      }
      else
        map.put("state_s", "unknown");

      n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_flags"));
      map.put("flags", Integer.toString(n));

      n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_prio"));
      map.put("prio", Integer.toString(n));

      try {
        n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_refs"));
        map.put("refs", Integer.toString(n));
      } catch (DebugProxyException e) {
        map.put("refs", "-");
      }

      try {
        n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_time"));
        map.put("time", Integer.toString(n));
      } catch (DebugProxyException e) {
        map.put("time", "-");
      }

      try {
        n = HexUtils.parseInt(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_u.wtobjp"));
        map.put("u", Integer.toString(n));
      } catch (DebugProxyException e) {
        map.put("u", "-");
      }

      // Inserting the new thread map into the threads list.
      lhm.put(current, map);

      previous = current;
    }
    return lhm;
  }
}
