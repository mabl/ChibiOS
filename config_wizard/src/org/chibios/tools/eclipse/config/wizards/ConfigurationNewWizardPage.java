package org.chibios.tools.eclipse.config.wizards;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Combo;
import org.osgi.framework.Bundle;

import config_wizard.Activator;

/**
 * The "New" wizard page allows setting the container for the new file as well
 * as the file name. The page will only accept file name without the extension
 * OR with the extension that matches the expected one (chcfg).
 */

public class ConfigurationNewWizardPage extends WizardPage {

  private Combo configurationTemplatesCombo;
  private Text fileText;

  private ISelection selection;
  private IContainer container;

  /**
   * Constructor for SampleNewWizardPage.
   * 
   * @param pageName
   */
  public ConfigurationNewWizardPage(ISelection selection) {

    super("wizardPage");
    setTitle("ChibiOS/RT Configuration Project File");
    setDescription("This wizard creates ChibiOS/RT configuration project represented by a .chcfg file. Configuration projects allow to generate project resources starting from high level descriptions written in XML.");
    this.selection = selection;
  }

  /**
   * @see IDialogPage#createControl(Composite)
   */
  public void createControl(Composite parent) {

    Composite container = new Composite(parent, SWT.NULL);
    GridLayout layout = new GridLayout();
    container.setLayout(layout);
    layout.numColumns = 2;
    layout.verticalSpacing = 9;

    Label lblConfigurationTemplate = new Label(container, SWT.NULL);
    lblConfigurationTemplate.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER,
                                                        false, false, 1, 1));
    lblConfigurationTemplate.setText("Configuration template:");

    configurationTemplatesCombo = new Combo(container, SWT.NONE);
    configurationTemplatesCombo.setLayoutData(new GridData(SWT.FILL,
                                                           SWT.CENTER, true,
                                                           false, 1, 1));
    populateConfigurationTemplatesCombo(configurationTemplatesCombo);

    Label label = new Label(container, SWT.NULL);
    label.setText("&File name:");

    fileText = new Text(container, SWT.BORDER | SWT.SINGLE);
    fileText.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
    fileText.addModifyListener(new ModifyListener() {
      public void modifyText(ModifyEvent e) {
        dialogChanged();
      }
    });

    initialize();
    dialogChanged();
    setControl(container);
  }

  /**
   * Tests if the current workbench selection is a suitable container to use.
   */

  private void initialize() {

    if (selection != null && selection.isEmpty() == false
        && selection instanceof IStructuredSelection) {
      IStructuredSelection ssel = (IStructuredSelection) selection;
      if (ssel.size() > 1)
        return;
      Object obj = ssel.getFirstElement();
      if (obj instanceof IResource) {
        if (obj instanceof IContainer)
          container = (IContainer) obj;
        else
          container = ((IResource) obj).getParent();
        // containerText.setText(container.getFullPath().toString());
      }
    }
    fileText.setText("config.chcfg");
  }

  /**
   * Ensures that both text fields are set.
   */

  private void dialogChanged() {
    IResource container = ResourcesPlugin.getWorkspace().getRoot()
        .findMember(new Path(getContainerName()));
    String fileName = getFileName();

    if (getContainerName().length() == 0) {
      updateStatus("File container must be specified");
      return;
    }
    if (container == null
        || (container.getType() & (IResource.PROJECT | IResource.FOLDER)) == 0) {
      updateStatus("File container must exist");
      return;
    }
    if (!container.isAccessible()) {
      updateStatus("Project must be writable");
      return;
    }
    if (fileName.length() == 0) {
      updateStatus("File name must be specified");
      return;
    }
    if (fileName.replace('\\', '/').indexOf('/', 1) > 0) {
      updateStatus("File name must be valid");
      return;
    }
    int dotLoc = fileName.lastIndexOf('.');
    if (dotLoc != -1) {
      String ext = fileName.substring(dotLoc + 1);
      if (ext.equalsIgnoreCase("chcfg") == false) {
        updateStatus("File extension must be \"chcfg\"");
        return;
      }
    }
    updateStatus(null);
  }

  private void updateStatus(String message) {
    setErrorMessage(message);
    setPageComplete(message == null);
  }

  private void populateConfigurationTemplatesCombo(Combo configurationTemplateCombo) {
    URL url1, url2;

    Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID);
    Path path = new Path("resources/gencfg/processors/processors.properties");
    url1 = FileLocator.find(bundle, path, null);
    try {
      url2 = FileLocator.toFileURL(url1);
    } catch (IOException e1) {
      // TODO Auto-generated catch block
      e1.printStackTrace();
      return;
    }
    String s = url2.getFile();
    try {
      FileInputStream fs = new FileInputStream(s);
    } catch (FileNotFoundException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
    return;
  }

  public String getContainerName() {

    return container.getFullPath().toString();
  }

  public String getFileName() {
    return fileText.getText();
  }
}
