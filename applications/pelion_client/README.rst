.. _pelion_client:

Pelion client
#############

.. contents::
   :local:
   :depth: 2

The Pelion client demonstrates the integration of the Pelion library within the |NCS|.
It creates two standard OMA resources to show communication with the Pelion cloud platform.
One OMA resource reacts to button presses, while the other implements the stopwatch counter.
The state of both resources can be tracked in the cloud platform.

.. note::
    The code is currently a work in progress and is not fully optimized yet.
    It will undergo changes and improvements in the future.

Pelion is an IoT cloud platform, initially created by Arm.
It offers services in the fields of connectivity, device management, and edge computing, which allow you to remotely manage and update your IoT devices.
To read more, see the `Pelion`_ website and the `Pelion documentation`_ page.

The Pelion client application integrates `Device Management`_ features of the Pelion platform, namely transport and firmware update deployment.

Overview
********

# PROVISIONING

At boot, the application opens the flash storage (fcc_init) and, unless you decide to reset the flash storage (fcc_storage_delete), starts in Pelion's developer flow and attempts to load Pelion's bootstrap data (fcc_developer_flow).
The bootstrap data is used to access the Pelion cloud.
A successful connection triggers the generation of the device identity in the Pelion cloud, which is then used for subsequent secured connection with the cloud.

# PELION INIT

The application then starts the Pelion module, which after initialization attaches several client callbacks required for informing the application about the connection status with the Pelion cloud.
, which wraps these callbacks into Pelion state events

PELION_STATE_DISABLED
PELION_STATE_INITIALIZED
PELION_STATE_REGISTERED

# CREATION OF RES

The application initializes the cloud client.
It then triggers the creation of objects (create_object_event) with their related OMA resources.
Each module used by the application creates the resources using the M2MInterfaceFactory.
For the Pelion client application, these are OMA Digital Input (oma_digital_input.cpp) and OMA Stopwatch (oma_stopwatch.cpp).

http://www.openmobilealliance.org/wp/omna/lwm2m/lwm2mregistry.html
https://raw.githubusercontent.com/OpenMobileAlliance/lwm2m-registry/prod/3200.xml

# NETWORK CONNECTION AND SETUP

The cloud client is then set up and starts communicating with the network, either LTE or OpenThread.
Pelion is started and the whole communication is done through sockets.

# COMMUNICATION

Pelion then sends the communication within Pelion.
You can locate the device in Pelion's device directory and start interacting with it.

# PERFORMING UPDATE

When the Pelion library receives the update image and an update campaign is scheduled in Pelion cloud, all devices for the given campaign can receive the update image.
See LINK TO PELION DOCS for details.





The application builds the Pelion resources for the given development kit.
It then connects to the Pelion platform and sends data to the web server over XYZ.

The application shares resources of each of the supported development kits with the Pelion platform instance.
This instance gives you access to Device Management Update functionalities, including ...

Pelion platform integration
===========================

Pelion creates keys to secure connection with the Pelion network using certificate keys.
This enables sending firmware updates and getting data from the cloud in a secure manner.
For this to work, Pelion needs to provision the Pelion client application with keys that secure the connection, taken from the bootstrap device certificate table in .c.
This is done with the developer flow (fcc developer flow) or with the production tool.

The application uses Pelion's *developer flow*.
Once logged to the Pelion platform, you need to download and add the certificate with developer credentials, which is populated when downloaded: https://github.com/nrfconnect/sdk-nrf/blob/master/applications/pelion_client/configuration/common/mbed_cloud_dev_credentials.c

Memory allocation (for Pelion library)
======================================

The application uses a flash sector for Pelion data storage.
This sector is used to store Pelion credentials, application information, and update data.
It is managed by Pelion's file data system.
The application uses Pelion's functions to populate this flash sector (fcc_developer_flow()).
If the sector is not empty, the application does not perform any action.





Firmware architecture
=====================

The Pelion client application has a modular structure, where ...

The following figure shows ...

.. figure:: /images/asset_tracker_v2_module_hierarchy.svg
    :alt: Module hierarchy

    Relationship between modules and the event manager

Transport structure
===================

Pelion is started and the whole communication is done through sockets.


Device Firmware Update
======================

When the Pelion library receives the update image and an update campaign is scheduled in Pelion cloud, all devices for the given campaign can receive the update image.
See LINK TO PELION DOCS for details.

Device modes
============

Data buffers
============

User Interface
**************

LED1 -- network connection status (breathing = in progress, solid = conncted)
LED2 -- pelion connection status ( ", " )

Button 1 -- 
Button 2 -- 

Requirements
************

The application supports the following development kits:

.. table-from-rows:: /includes/sample_board_rows.txt
   :header: heading
   :rows: nrf9160dk_nrf9160ns, nrf5340dk_nrf5340_cpuapp, nrf52840dk_nrf52840

.. include:: /includes/spm.txt

Configuration
*************

For the Pelion client application to work, you need to set up both the application and the Pelion web platform.

Pelion web platform setup
=========================

The application requires you to generate and download a developer certificate from the Pelion web service.

Follow steps: https://developer.pelion.com/docs/device-management/current/provisioning-process/provisioning-development-devices.html

Application setup
=================

* :ref:`nrfxlib:nrf_security`


Configuration options
---------------------

Check and configure the following configuration options for the application:


Additional configuration
------------------------

Additionally, you can add the following optional configurations:

Configuration files (verify)
----------------------------

The application provides predefined configuration files for typical use cases.
You can find the configuration files in the :file:`applications/pelion_client/` directory.

It is possible to build the application with overlay files for both DTS and Kconfig to override the default values for the board.
The application contains examples of Kconfig overlays.

The following configuration files are available in the application folder:

* :file:`prj.conf` - Configuration file common for all build targets
* :file:`boards/thingy91_nrf9160ns.conf` - Configuration file specific for Thingy:91. The file is automatically merged with :file:`prj.conf` when you build for the ``thingy91_nrf9160ns`` build target.
* :file:`overlay-low-power.conf` - Configuration file that achieves the lowest power consumption by disabling features  that consume extra power like LED control and logging.
* :file:`overlay-debug.conf` - Configuration file that adds additional verbose logging capabilities to the application

Generally, Kconfig overlays have an ``overlay-`` prefix and a ``.conf`` extension.
Board-specific configuration files are placed in the :file:`boards` folder and are named as :file:`<BOARD>.conf`.
DTS overlay files are named the same as the build target and use the file extension ``.overlay``.
When the DTS overlay filename matches the build target, the overlay is automatically chosen and applied by the build system.

Building and running
********************

Before building and running the firmware, ensure that the Pelion cloud platform is set up.
Also, the device must be provisioned and configured with the certificates according to the instructions for the respective cloud for the connection attempt to succeed.

.. note::

   This application supports :ref:`mcuboot:mcuboot_wrapper`, which is enabled by default.


.. |sample path| replace:: :file:`applications/pelion_client`
.. include:: /includes/build_and_run_nrf9160.txt

Building with overlays
======================

To build with Kconfig overlay, it must be based to the build system, as shown in the following example:

    ``west build -b nrf9160dk_nrf9160ns -- -DOVERLAY_CONFIG=overlay-low-power.conf``

The above command will build for nRF9160 DK using the configurations found in :file:`overlay-low-power.conf`, in addition to the configurations found in :file:`prj_nrf9160dk_nrf9160ns.conf`.
If some options are defined in both files, the options set in the overlay take precedence.

Testing
=======

After programming the application and all the prerequisites to your development kit, test the application by performing the following steps:

1. |connect_kit|
#. Connect to the kit with a terminal emulator (for example, LTE Link Monitor). See :ref:`lte_connect` for more information.
#. Reset the development kit.


Dependencies
************

This application uses the following Zephyr libraries:

* :ref:`zephyr:thread_protocol_interface`

This application uses the following |NCS| libraries and drivers:

* :ref:`lib_caf`
* :ref:`event_manager`
* :ref:`lte_lc_readme`
* :ref:`partition_manager`

It uses the following `sdk-nrfxlib`_ library:

* :ref:`nrfxlib:nrf_modem`
* :ref:`nrfxlib:nrf_security`

It uses the following `sdk-mcuboot`_ library:

* :ref:`MCUboot <mcuboot:mcuboot_wrapper>`

In addition, it uses the following sample:

* :ref:`secure_partition_manager`

.. _pelion_client_internal_modules:

Internal modules
****************

* CAF buttons
* CAF LEDs

* power manager -- as in nRF Desktop

* net module, in two variants: net LTE and net OT; both monitor the state of their network and send net_state_connected or net_state_disconnected.
  it only serves the purpose of checking the connection state based on callbacks and information sent by the networks.
  
* Pelion module - follows Pelion state: 1) initialized, 2) objects connected, 3) pinging network, 4) confirming registration, 5) depending on changes to objects, exchanging info about objects

* Pelion FCC (factory config client) - should serve only debugging; used to add the factory default configuration; takes the hardcoded values from the config and puts it into the app.
  if there is a need for a custom configuration, user needs to create its own module.

* oma (open mobile alliance) digital input & oma stopwatch - demonstrate the two types of objects according to lwm2m standards;
  
  * digital input enables buttons (when starting the board, the firmware registers 4 buttons in Pelion cloud; then, it registers the status pushed/unpushed and n.o of pushes -- added only for showcasing integration)
  * stopwatch counts time, increasing it periodically (configurable) and it can be reset

.. _pelion_client_memory_allocation:

Memory allocation
=================

Mostly, the modules use statically allocated memory.
Following are some features that rely on dynamically allocated memory, using the :ref:`Zephyr heap memory pool implementation <zephyr:heap_v2>`:

* Event manager events
* Pelion library
* mbedTLS

You can configure the heap memory by using the :option:`CONFIG_HEAP_MEM_POOL_SIZE`.
