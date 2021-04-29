.. _pelion_client:

Pelion client
#############

.. contents::
   :local:
   :depth: 2

The Pelion client application demonstrates the integration of the Pelion library within the |NCS|.
The application builds the Pelion resources, connects to the Pelion platform and sends data to the web server over either LTE or OpenThread.

Pelion is an IoT cloud platform that offers services in the fields of connectivity, device management, and edge computing, which allow you to remotely manage and update your IoT devices.
To read more, see the `Pelion`_ website and the `Pelion documentation`_ portal.

Overview
********

The Pelion client application integrates `Pelion Device Management`_ features.
The application establishes a secure connection with the Pelion cloud platform, starts the Pelion module and initializes the cloud client.
The client creates two standard OMA resources that you can interact with from the Pelion web application.
One OMA resource reacts to button presses, while the other implements the stopwatch counter.
The Pelion client then starts communicating with the network using sockets.

Operational flow overview
=========================

The following diagram and sections describe the Pelion client application operational flow in details:

.. figure:: /images/pelion_client_flow.svg
    :alt: Pelion client application operational flow

    Pelion client application operational flow

Application boot (provisioning)
-------------------------------

At boot, the application sets up the device for secure connection and communication with Pelion Device Management.
This is part of Pelion's device provisioning process, which can be done either using a production tool (Factory Configurator Utility for factory provisioning) or the developer mode (for development devices).
For more information about the process, see `Provisioning devices for Pelion Device Management`_ in the Pelion documentation.

The application follows Pelion's developer mode process. 
It first opens the flash storage (:c:func:`fcc_init`) and, unless you decide to reset the flash storage (using :c:func:`fcc_storage_delete`), it starts in Pelion's developer mode.
It then attempts to load Pelion's bootstrap data (:c:func:`fcc_developer_flow`).
The bootstrap data is used to access the Pelion cloud and lets you send firmware updates and get data from the cloud.
A successful connection triggers the generation of the device identity in the Pelion cloud, which is used for establishing a secured connection.

Pelion module initialization
----------------------------

After connecting with the cloud, the application starts the Pelion module.
Once initialized, the module attaches several client callbacks that are required for informing the application about the connection status with the Pelion cloud.
It then wraps these callbacks into `Pelion client state events`_.

OMA resource creation
---------------------

The application initializes the cloud client.
It then triggers the creation of objects (create_object_event) with their related OMA resources.
Each module used by the application creates the resources using the M2MInterfaceFactory.
For the Pelion client application, these are OMA Digital Input (oma_digital_input.cpp) and OMA Stopwatch (oma_stopwatch.cpp).

http://www.openmobilealliance.org/wp/omna/lwm2m/lwm2mregistry.html
https://raw.githubusercontent.com/OpenMobileAlliance/lwm2m-registry/prod/3200.xml

Network connection and setup
----------------------------

The cloud client is then set up and starts communicating with the network, either LTE or OpenThread.
Pelion is started and the whole communication is done through sockets.

Network communication
---------------------

Pelion then sends the communication within Pelion.
You can locate the device in Pelion's device directory and start interacting with it.

Firmware architecture
=====================

The Pelion client application has a modular structure, where ...

The following figure shows ...

.. figure:: /images/pelion_client_architecture.svg
    :alt: Module hierarchy

    Relationship between modules and the event manager

Transport structure
===================

Pelion is started and the whole communication is done through sockets.


Device Firmware Update
======================

When the Pelion library receives the update image and an update campaign is scheduled in Pelion cloud, all devices for the given campaign can receive the update image.
See LINK TO PELION DOCS for details.

Network modes
=============

OpenThread

LTE

Pelion client state events
==========================

.. figure:: /images/pelion_client_states.svg
    :alt: Pelion client states

    Pelion client states

PELION_STATE_DISABLED
PELION_STATE_INITIALIZED

PELION_STATE_REGISTERED >> PELION_STATE_UNREGISTERED

- when trying to send data, Pelion client can see it has no access to server, for example when LTE or OTBR are connected but are unable to connect to pelion. no reason for network failure.
- Pelion client is paused when we know reason for network failure

NET_STATE_DISABLED
NET_STATE_DISCONNECTED
NET_STATE_CONNECTED

User Interface
**************

The application uses the following buttons on the supported development kits:

* Button 1 -- Increase the value of the OMA Digital Input resource.
* Button 2 -- Decrease the value of the OMA Digital Input resource.

Additionally, the application displays LED behavior that corresponds to the task performed by the application.
The following table shows the LED behavior demonstrated by the application:


+-------------------------------+-----------------------+
| State                         | DK LEDs               |
+===============================+=======================+
| Network connection search     | LED1 blinking         |
+-------------------------------+-----------------------+
| Pelion connection search      | LED2 blinking         |
+-------------------------------+-----------------------+
| Network connection established| LED1 solid on         |
+-------------------------------+-----------------------+
| Pelion connection established | LED2 solid on         |
+-------------------------------+-----------------------+

+-------------------------------+-----------------------+
| State                         | DK LEDs               |
+===============================+=======================+
| Network connection search     | LED1 blinking         |
+-------------------------------+-----------------------+
| Pelion connection search      | LED2 blinking         |
+-------------------------------+-----------------------+
| Network connection established| LED1 solid on         |
+-------------------------------+-----------------------+
| Pelion connection established | LED2 solid on         |
+-------------------------------+-----------------------+


Requirements
************

The application supports the following development kits:

.. table-from-rows:: /includes/sample_board_rows.txt
   :header: heading
   :rows: nrf9160dk_nrf9160ns, nrf5340dk_nrf5340_cpuapp, nrf52840dk_nrf52840

.. include:: /includes/spm.txt

Pelion Device Management requirements
=====================================

You also need a developer account on Pelion's `Device Management Portal`_.

Configuration
*************

For the Pelion client application to work, you need to set up both the application and the Pelion web platform.

Pelion Device Management setup
==============================

To be able to connect to the Pelion cloud, you need to complete the following configuration:

* Generate and download a developer certificate file from Pelion's Device Management Portal.
  This file contains keys used for securing connection with the Pelion network.
* Add the developer certificate to your application by adding the following functions:

  * :c:func:`fcc_init()`
  * :c:func:`fcc_developer_flow()`
  
  Both these functions enable Pelion's developer mode.

For details steps, see the `Provisioning development devices`_ guide in the Pelion documentation.

As a result of this setup, you will have the following:

* The populated credential file :file:`mbed_cloud_dev_credentials.c` that contains the bootstrap device certificate table added to your build configuration.
* The developer mode enabled and the development certificate used in your software image.

Application setup
=================

The following options related to Pelion library need to be always enabled for the application to work:

* CONFIG_PELION_CLIENT -- You need to set CONFIG_PELION_CLIENT=y to enable the Pelion library, which is required for the application to work.
* CONFIG_PELION_NRF_SECURITY -- enables the default mbedtls configuration for Pelion related to :ref:`nrfxlib:nrf_security`.

This Kconfig option is enabled by default for each supported board's configuration file.

Additional configuration
------------------------

Additionally, you can add the following optional configurations:

* CONFIG_PELION_UPDATE_DEVELOPER -- enables the automatic insertion of the :file:`update_default_resources.c`.
  This is required for the DFU.
* CONFIG_PELION_UPDATE

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
#. Connect to the kit with a terminal emulator (for example, LTE Link Monitor).
   See :ref:`lte_connect` for more information.
#. Reset the development kit.
#. Turn on the development kit.
   LED1 and LED2 will start blinking, which indicates the network is connecting to the Pelion cloud.
   After several seconds, the LEDs stop blinking and become solid, which indicates that the connection is established.
#. Log in to Pelion Device Management platform.
#. Go to :guilabel:`section` -> :guilabel:`section` -> :guilabel:`section`.
#. Select your device from the list of devices.
#. In the XYZ tab, scroll down to where the value for the OMA Digital Input resource is listed.
#. Press Button 1.
   The value in the Pelion Device Management platform will increase.
#. Scroll down to where the value for the OMA Stopwatch resource is listed.
#. Wait for a couple of seconds to see how the value increases.


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

The Pelion client application uses the following modules from the |NCS|:

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

* dfu

.. _pelion_client_memory_allocation:

Memory allocation
=================

The application uses a flash sector for Pelion data storage.
This sector is used to store Pelion credentials, application information, and update data.
It is managed by Pelion's file data system.
The application uses Pelion's functions to populate this flash sector (fcc_developer_flow()).
If the sector is not empty, the application does not perform any action.

Mostly, the modules use statically allocated memory.

.. figure:: /images/pelion_client_memory.svg
    :alt: Flash memory allocation structure

Following are some features that rely on dynamically allocated memory, using the :ref:`Zephyr heap memory pool implementation <zephyr:heap_v2>`:

* Event manager events
* Pelion library
* mbedTLS

You can configure the heap memory by using the :option:`CONFIG_HEAP_MEM_POOL_SIZE`.

The following table contains the flash memory addresses for the flash memory sectors per platform.

+--------------------+---------------+-------------+
| Sector             | Address       | nRF9160 DK  |
+====================+===============+=============+
| MCUboot            | Start address | 0x000ABC    |
|                    +---------------+-------------+
|                    | End address   | 0x000ABC    |
+--------------------+---------------+-------------+
| SPM                | Start address | 0x000ABC    |
|                    +---------------+-------------+
|                    | End address   | 0x000ABC    |
+--------------------+---------------+-------------+
| Application        | Start address | 0x000ABC    |
|                    +---------------+-------------+
|                    | End address   | 0x000ABC    |
+--------------------+---------------+-------------+
| Settings           | Start address | 0x000ABC    |
|                    +---------------+-------------+
|                    | End address   | 0x000ABC    |
+--------------------+---------------+-------------+
| Pelion credentials | Start address | 0x000ABC    |
|                    +---------------+-------------+
|                    | End address   | 0x000ABC    |
+--------------------+---------------+-------------+


https://github.com/nrfconnect/sdk-nrf/blob/master/applications/pelion_client/configuration/nrf52840dk_nrf52840/pm_static_ZDebug.yml
https://github.com/nrfconnect/sdk-nrf/blob/master/applications/pelion_client/configuration/nrf9160dk_nrf9160ns%401.0.0/pm_static_ZDebug.yml
https://github.com/nrfconnect/sdk-nrf/blob/master/applications/pelion_client/configuration/nrf5340dk_nrf5340_cpuapp/pm_static_ZDebug.yml
https://github.com/nrfconnect/sdk-nrf/blob/master/applications/pelion_client/configuration/nrf5340dk_nrf5340_cpuappns/pm_static_ZDebug.yml
