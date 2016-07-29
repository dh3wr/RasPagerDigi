project(raspagercontrol)

add_executable(raspagercontrol menus/settings/ipeingabe.cpp menus/settings/ipadresseeingabe.cpp menus/settings/gatewayeingabe.cpp menus/settings/subneteingabe.cpp menus/settings/dnseingabe.cpp tools/numberhandler.cpp menus/system/settime.cpp menus/system/linuxupdate.cpp menus/system/shutdown.cpp menus/system/restart.cpp menus/menuiteminfo.cpp menus/menuitemokback.cpp menus/menuitem.cpp menus/menuiteminforotator.cpp menus/info/network.cpp menus/info/ntp.cpp menus/info/hardware.cpp tools/networkcontrol.cpp tools/systemcontrol.cpp tools/raspagerdigiextension.cpp main.cpp)

add_subdirectory(menus)
add_subdirectory(tools)

set(CMAKE_CXX_FLAGS "-lwiringPi")