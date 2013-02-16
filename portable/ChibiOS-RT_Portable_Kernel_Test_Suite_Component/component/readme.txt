*****************************************************************************
*** Component directories organization.                                   ***
*****************************************************************************

--{plugin}                      - SPC5 component plugin root directory.
  +--component/                 - Component source and resource files.
     +--cfg/                    - Configurable source or resource files. The
     |                            files under this directory are processed as
     |                            templates and are generated into the final
     |                            project under:
     |                                  ./cfg/<component_name>
     +--doc/                    - Component documents, the documents are not
     |                            copied in the final project but can be
     |                            accessed through the Eclipse IDE.
     +--lib/                    - Source or resource static files. The files
     |                            in this directory are linked from (or copied
     |                            to) the final project under:
     |                                  ./lib/<component_name>
     |                            Note that <component_name> can be a physical
     |                            directory or a symbolic link depending on
     |                            the project settings.
     +--mak/                    - Component build-related files.
     |  +--component.mak.ftl    - Template of the component makefile segment.
     |                            This file is exploded in the final project
     |                            under:
     |                                  ./mak/<component_name>.mak
     +--root_wa/                - "Write Always" configurable source or
     |                            resource files. The files under this
     |                            directory are processed as templates and
     |                            are generated into the final project under
     |                            the root. The files are re-generated each
     |                            time the application saves a configuration.
     +--root_wo/                - "Write Once" configurable source or
     |                            resource files. The files under this
     |                            directory are processed as templates and
     |                            are generated into the final project under
     |                            the root. The files are generated only the
     |                            first time the application wizard saves a
     |                            configuration for a component or if a
     |                            re-generation is enforce.
     +--readme.txt              - This file.
