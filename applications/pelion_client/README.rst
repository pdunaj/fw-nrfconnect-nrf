.. _pelion_client:

Pelion client
#############

.. contents::
   :local:
   :depth: 2

The Pelion client demonstrates the integration of the Pelion library within the |NCS|.
It creates two standard OMA resources to show communication with the Pelion cloud platform.
One OMA resource reacts to button presses, while the other implements the stopwatch counter.
The state of both resources can be tracked online.

.. note::
    The code is currently a work in progress and is not fully optimized yet.
    It will undergo changes and improvements in the future.

Overview
********

The application is intended to be used with an instance of the Pelion platform running on the cloud side.

The application builds the Pelion resources for the given development kit.
It then connects to the Pelion platform and sends data to the web server over XYZ.

The application shares resources of each of the supported development kits with the Pelion platform instance.
This instance gives you access to Device Management Update functionalities, including ...

Pelion platform integration
===========================

Pelion is an IoT cloud platform, initially created by Arm.
It offers services in the fields of connectivity, device management, and edge computing, which allow you to remotely manage and update your IoT devices.
To read more, see the `Pelion`_ website and the `Pelion documentation`_ page.

The Pelion client application integrates `Device Management`_ features of the Pelion platform, namely transport and firmware update deployment.

Firmware architecture
=====================

The Pelion client application has a modular structure, where ...

The following figure shows ...

.. figure:: /images/asset_tracker_v2_module_hierarchy.svg
    :alt: Module hierarchy

    Relationship between modules and the event manager

Transport structure
===================

Device Firmware Update
======================

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

Before building and running the firmware ensure that the cloud side is set up.
Also, the device must be provisioned and configured with the certificates according to the instructions for the respective cloud for the connection attempt to succeed.

.. note::

   This application supports :ref:`ug_bootloader`, which is disabled by default.
   To enable the immutable bootloader, set ``CONFIG_SECURE_BOOT=y``.


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
