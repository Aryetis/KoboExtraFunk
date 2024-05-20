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
# Below code, copy pasted from https://github.com/koreader/koreader/blob/master/platform/kobo/obtain-ip.sh    #
# credits to Niluje and koreader's team                                                                       #
###############################################################################################################

# NOTE: Close any non-standard fds, so that it doesn't come back to bite us in the ass with USBMS later...
for fd in /proc/"$$"/fd/*; do
    fd_id="$(basename "${fd}")"
    if [ -e "${fd}" ] && [ "${fd_id}" -gt 2 ]; then
        # NOTE: dash (meaning, in turn, busybox's ash, uses fd 10+ open to /dev/tty or $0 (w/ CLOEXEC))
        fd_path="$(readlink -f "${fd}")"
        if [ "${fd_path}" != "/dev/tty" ] && [ "${fd_path}" != "$(readlink -f "${0}")" ] && [ "${fd}" != "${fd_path}" ]; then
            eval "exec ${fd_id}>&-"
            echo "[obtain-ip.sh] Closed fd ${fd_id} -> ${fd_path}"
        fi
    fi
done

#./release-ip.sh, guess what, credits to Niluje and koreader's team, https://github.com/koreader/koreader/blob/master/platform/kobo/release-ip.sh
###############################################################################################################
# Release IP and shutdown udhcpc.
# NOTE: Save our resolv.conf to avoid ending up with an empty one, in case the DHCP client wipes it on release (#6424).
cp -a "/etc/resolv.conf" "/tmp/resolv.ko"
old_hash="$(md5sum "/etc/resolv.conf" | cut -f1 -d' ')"

if [ -x "/sbin/dhcpcd" ]; then
    dhcpcd -d -k "${INTERFACE}"
    killall -q -TERM udhcpc default.script
else
    killall -q -TERM udhcpc default.script dhcpcd
    ifconfig "${INTERFACE}" 0.0.0.0
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
###############################################################################################################

# NOTE: Prefer dhcpcd over udhcpc if available. That's what Nickel uses,
#       and udhcpc appears to trip some insanely wonky corner cases on current FW (#6421)
if [ -x "/sbin/dhcpcd" ]; then
    dhcpcd -d -t 30 -w "${INTERFACE}"
else
    udhcpc -S -i "${INTERFACE}" -s /etc/udhcpc.d/default.script -b -q
fi
