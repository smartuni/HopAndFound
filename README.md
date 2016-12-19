# HopAndFound #

WS 2016/17 - RIOT im Internet of Things - Bachelor Project of Computer Science and Electrical Engineering

Hop And Found is a burglar alarm for crowded places like concerts. It is made to help the user monitor items he does not want to be stolen, like his valuables. A component is placed on the item to monitor and the user carries a component which tells him when the item is being stolen.

Hop And Found consists of three types of components:

* **monitor:**            Component with a display which the user carries. It signals the user if the device is lost.
* **monitored item:**     Component which is placed with the item that is monitored.
* **node:**               Stationary component which is placed at known positions in an area for forwarding messages.

## Hardware ##

The board of all components is the [Phytec phyWAVE KW22](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-Phytec-phyWAVE-KW22).
The display of the monitor is the Nokia pcd8544.

## Messages ##

* **heartbeat:**                Sent from the monitor to the monitored item to ensure the distance between the two isn't too great.
* **localization request:**     Sent to the nodes to determine which nodes are in range.
* **localization reply:**       Answer to the localization request to confirm node is in range.
* **call for help:**            Sent by the monitored item and forwarded by the nodes. Contains information about the whereabouts of the monitored item.

## I/O ##

#### monitor ####

* LED blinks red if monitored item goes missing.
* User button turns off the sending of heartbeats.

#### monitored item ####

* LED blinks red if heartbeat is not received in time.
* User button links monitored item to monitor when close to each other.

#### node ####

* LED blinks blue when localization request is received.
* User button sends localization request to other nodes.

## Setup ##

#### general ####

In "src/include/globals.h":

* Netif channel can be changed.
* Signal strength can be adjusted.
* Debug settings can be toggled.
* Max number of nodes can be set.

#### monitor ####

* Go into "src/monitor/" and compile and flash.

#### monitored item ####

* Set monitored item ID in "src/include/globals.h".
* Go into "src/monitored_item/" and compile and flash.
* Put the monitor and monitored item close to each other and press the user button on the monitored item to link it with the monitor.

#### node ####

* Set node ID in "src/include/globals.h".
* Go into "src/node/" and compile and flash.
* The nodes should be set up in a way that they reach at least two other nodes. To help achieve this the user button of the node may be used. When pressed, the LED of the nodes which are in range will blink blue.

## Procedure when monitored item goes missing ##

* Monitored item moves too far away from the monitor and thereby doesn't receive any more heartbeats.
* Monitored item sends a localization request.
* All nodes in range answer with a localization request.
* Monitored item sends a call for help.
* Nodes forward the call for help until it reaches the monitor.
* Upon receiving the call for help, the monitor displays the information about the location of the monitored item contained in the call for help.
