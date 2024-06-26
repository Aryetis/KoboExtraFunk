#!/bin/sh

# Dump the NTX HWConfig block, cf : https://github.com/NiLuJe/FBInk/blob/012dd73784969a05095400137b358ef43522aed5/utils/devcap_test.sh#L49-L67
if [ -e "/dev/disk/by-partlabel/hwcfg" ] && [ "$(dd if=/dev/disk/by-partlabel/hwcfg bs=512 skip=1 count=1 2>/dev/null | head -c 9)" = "HW CONFIG" ] ; then
	# Modern variant
	WIFI=$(ntx_hwconfig -S 1 /dev/disk/by-partlabel/hwcfg | grep Wifi | awk -F "\'" '{print $2}')
elif [ "$(dd if=/dev/mmcblk0 bs=512 skip=1024 count=1 2>/dev/null | head -c 9)" = "HW CONFIG" ] ; then
	ntx_hwconfig -s /dev/mmcblk0 >> "${DEVCAP_LOG}" 2>/dev/null
        WIFI=$(ntx_hwconfig -s /dev/mmcblk0 2>/dev/null | grep Wifi | awk -F "\'" '{print $2}')
elif [ -e "/dev/mmcblk0p6" ] && [ "$(dd if=/dev/mmcblk0p6 bs=512 skip=1 count=1 2>/dev/null | head -c 9)" = "HW CONFIG" ] ; then
	ntx_hwconfig -S 1 /dev/mmcblk0p6 >> "${DEVCAP_LOG}" 2>/dev/null
        WIFI=$(ntx_hwconfig -S 1 /dev/mmcblk0p6 2>/dev/null | grep Wifi | awk -F "\'" '{print $2}')
else
	echo "Couldn't find a HWConfig tag?! exiting..."
	exit
fi

# cf Kobo's /etc/init.d/rcS
if [ $PLATFORM == freescale ]; then
        INTERFACE=wlan0
        WIFI_MODULE=ar6000
else
        INTERFACE=eth0
        WIFI_MODULE=dhd
        if [ x$WIFI == "xRTL8189" ]; then
                WIFI_MODULE=8189fs
        elif [ x$WIFI == "xRTL8192" ]; then
                WIFI_MODULE=8192es
        elif [ x$WIFI == "xRTL8821CS" ]; then
                INTERFACE=wlan0
                WIFI_MODULE=8821cs
        elif [ x$WIFI == "xRTL8723DS" ]; then
                INTERFACE=wlan0
                WIFI_MODULE=8723ds
        elif [ x$WIFI == "xCYW43455" ]; then
                INTERFACE=wlan0
                WIFI_MODULE=bcmdhd
        elif [ x$WIFI == "x88W8987" ]; then
                INTERFACE=mlan0
                WIFI_MODULE=moal
        elif [ x$WIFI == "xMT8113T" ]; then
                INTERFACE=wlan0
                WIFI_MODULE=wlan_drv_gen4m
        fi
fi

###############################################################################################################
# Below code, copy pasted from https://github.com/koreader/koreader/blob/master/platform/kobo/disable-wifi.sh #
# credits to Niluje and koreader's team                                                                       #
# replacing luajit calls by ioctl /dev/ntx_io 208                                                             #
###############################################################################################################

# Disable wifi, and remove all modules.
# NOTE: Save our resolv.conf to avoid ending up with an empty one, in case the DHCP client wipes it on release (#6424).
cp -a "/etc/resolv.conf" "/tmp/resolv.ko"
old_hash="$(md5sum "/etc/resolv.conf" | cut -f1 -d' ')"

if [ -x "/sbin/dhcpcd" ]; then
    dhcpcd -d -k "${INTERFACE}"
    killall -q -TERM udhcpc default.script
else
    killall -q -TERM udhcpc default.script dhcpcd
fi

# NOTE: dhcpcd -k waits for the signalled process to die, but busybox's killall doesn't have a -w, --wait flag,
#       so we have to wait for udhcpc to die ourselves...
# NOTE: But if all is well, there *isn't* any udhcpc process or script left to begin with...
kill_timeout=0
while pkill -0 udhcpc; do
    # Stop waiting after 5s
    if [ ${kill_timeout} -ge 20 ]; then
        break
    fi
    usleep 250000
    kill_timeout=$((kill_timeout + 1))
done

new_hash="$(md5sum "/etc/resolv.conf" | cut -f1 -d' ')"
# Restore our network-specific resolv.conf if the DHCP client wiped it when releasing the lease...
if [ "${new_hash}" != "${old_hash}" ]; then
    mv -f "/tmp/resolv.ko" "/etc/resolv.conf"
else
    rm -f "/tmp/resolv.ko"
fi

wpa_cli -i "${INTERFACE}" terminate

[ "${WIFI_MODULE}" = "dhd" ] && wlarm_le -i "${INTERFACE}" down
ifconfig "${INTERFACE}" down

# Handle dependencies, if any
WIFI_DEP_MOD=""
# Honor the platform's preferred method to toggle power
POWER_TOGGLE="module"
# Some platforms never unload the wifi modules
SKIP_UNLOAD=""
case "${WIFI_MODULE}" in
    "moal")
        WIFI_DEP_MOD="mlan"
        POWER_TOGGLE="ntx_io"
        ;;
    "wlan_drv_gen4m")
        POWER_TOGGLE="wmt"
        SKIP_UNLOAD="true"
        ;;
esac

if [ -z "${SKIP_UNLOAD}" ]; then
    # Some sleep in between may avoid system getting hung
    # (we test if a module is actually loaded to avoid unneeded sleeps)
    if grep -q "^${WIFI_MODULE} " "/proc/modules"; then
        usleep 250000
        # NOTE: Kobo's busybox build is weird. rmmod appears to be modprobe in disguise, defaulting to the -r flag...
        #       But since there's currently no modules.dep file being shipped, nor do they include the depmod applet,
        #       go with what the FW is doing, which is rmmod.
        # c.f., #2394?
        rmmod "${WIFI_MODULE}"
    fi

    if [ -n "${WIFI_DEP_MOD}" ]; then
        if grep -q "^${WIFI_DEP_MOD} " "/proc/modules"; then
            usleep 250000
            rmmod "${WIFI_DEP_MOD}"
        fi
    fi
fi

case "${POWER_TOGGLE}" in
    "ntx_io")
        usleep 250000
        ioctl /dev/ntx_io 208 -v 0
        #./luajit frontend/device/kobo/ntx_io.lua 208 0
        ;;
    "wmt")
        echo 0 >/dev/wmtWifi
        ;;
    *)
        if grep -q "^sdio_wifi_pwr " "/proc/modules"; then
            # Handle the shitty DVFS switcheroo...
            if [ -n "${CPUFREQ_DVFS}" ]; then
                echo "0" >"/sys/devices/platform/mxc_dvfs_core.0/enable"
                if [ -n "${CPUFREQ_CONSERVATIVE}" ]; then
                    echo "conservative" >"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
                else
                    echo "userspace" >"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
                    cat "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq" >"/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
                fi
            fi
            usleep 250000
            rmmod sdio_wifi_pwr
        fi

        # Poke the kernel via ioctl on platforms without the dedicated power module...
        if [ ! -e "/drivers/${PLATFORM}/wifi/sdio_wifi_pwr.ko" ]; then
            usleep 250000
            ioctl /dev/ntx_io 208 -v 0
            #./luajit frontend/device/kobo/ntx_io.lua 208 0
        fi
        ;;
esac
