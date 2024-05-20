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
# Below code, copy pasted from https://github.com/koreader/koreader/blob/master/platform/kobo/enable-wifi.sh  #
# credits to Niluje and koreader's team                                                                       #
# replacing luajit calls by ioctl /dev/ntx_io 208                                                             #
###############################################################################################################

# NOTE: Close any non-standard fds, so that it doesn't come back to bite us in the ass with USBMS later...
for fd in /proc/"$$"/fd/*; do
    fd_id="$(basename "${fd}")"
    if [ -e "${fd}" ] && [ "${fd_id}" -gt 2 ]; then
        # NOTE: dash (meaning, in turn, busybox's ash, uses fd 10+ open to /dev/tty or $0 (w/ CLOEXEC))
        fd_path="$(readlink -f "${fd}")"
        if [ "${fd_path}" != "/dev/tty" ] && [ "${fd_path}" != "$(readlink -f "${0}")" ] && [ "${fd}" != "${fd_path}" ]; then
            eval "exec ${fd_id}>&-"
            echo "[enable-wifi.sh] Closed fd ${fd_id} -> ${fd_path}"
        fi
    fi
done

# Some platforms do *NOT* use sdio_wifi_pwr, even when it is physically there...
POWER_TOGGLE="module"
# We also want to choose the wpa_supplicant driver depending on the module...
WPA_SUPPLICANT_DRIVER="wext"
# And some platforms also put the modules in some funkier paths...
KMOD_PATH="/drivers/${PLATFORM}/wifi"
case "${WIFI_MODULE}" in
    "moal")
        POWER_TOGGLE="ntx_io"
        WPA_SUPPLICANT_DRIVER="nl80211"
        ;;
    "wlan_drv_gen4m")
        POWER_TOGGLE="wmt"
        WPA_SUPPLICANT_DRIVER="nl80211"
        KMOD_PATH="/drivers/${PLATFORM}/mt66xx"
        ;;
esac

# Load the requested module if it isn't already
insmod_asneeded() {
    kmod="${1}"
    shift

    if ! grep -q "^${kmod} " "/proc/modules"; then
        insmod "${KMOD_PATH}/${kmod}.ko" "${@}"
        usleep 250000
    fi
}

# Power up WiFi chip
case "${POWER_TOGGLE}" in
    "ntx_io")
        # 208 is CM_WIFI_CTRL
        ioctl /dev/ntx_io 208 -v 1
        #./luajit frontend/device/kobo/ntx_io.lua 208 1
        ;;
    "wmt")
        # NOTE: Unlike earlier platforms, it seems the WiFi modules are only loaded once, and never unloaded.
        #       So, just make sure they actually are loaded before we go on...
        insmod_asneeded "wmt_drv"
        insmod_asneeded "wmt_chrdev_wifi"
        insmod_asneeded "wmt_cdev_bt"
        insmod_asneeded "${WIFI_MODULE}"

        # Black magic courtesy of wmt_dbg_func_ctrl @ (out of tree) modules/connectivity/wmt_mt66xx/common_main/linux/wmt_dbg.c
        # Enable debug commands
        echo "0xDB9DB9" >/proc/driver/wmt_dbg
        # Disable the LPBK test
        echo "7 9 0" >/proc/driver/wmt_dbg
        # Nickel appears to sleep for ~1s
        sleep 1
        echo "0xDB9DB9" >/proc/driver/wmt_dbg
        # Enable the LPBK test (this'll block for ~1.3s)
        echo "7 9 1" >/proc/driver/wmt_dbg
        # Finally, power on the chip
        echo 1 >/dev/wmtWifi
        ;;
    *)
        if ! grep -q "^sdio_wifi_pwr " "/proc/modules"; then
            if [ -e "${KMOD_PATH}/sdio_wifi_pwr.ko" ]; then
                # Handle the shitty DVFS switcheroo...
                if [ -n "${CPUFREQ_DVFS}" ]; then
                    echo "userspace" >"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
                    echo "1" >"/sys/devices/platform/mxc_dvfs_core.0/enable"
                fi

                insmod "${KMOD_PATH}/sdio_wifi_pwr.ko"
            else
                # Poke the kernel via ioctl on platforms without the dedicated power module...
                ioctl /dev/ntx_io 208 -v 1
                #./luajit frontend/device/kobo/ntx_io.lua 208 1
            fi
        fi
        ;;
esac
# Moar sleep!
usleep 250000

# Load WiFi modules
# NOTE: Used to be exported in WIFI_MODULE_PATH before FW 4.23
if ! grep -q "^${WIFI_MODULE} " "/proc/modules"; then
    # Set the Wi-Fi regulatory domain properly if necessary...
    WIFI_COUNTRY_CODE_PARM=""
    if grep -q "^WifiRegulatoryDomain=" "/mnt/onboard/.kobo/Kobo/Kobo eReader.conf"; then
        WIFI_COUNTRY_CODE="$(grep "^WifiRegulatoryDomain=" "/mnt/onboard/.kobo/Kobo/Kobo eReader.conf" | cut -d '=' -f2)"

        case "${WIFI_MODULE}" in
            "8821cs")
                WIFI_COUNTRY_CODE_PARM="rtw_country_code=${WIFI_COUNTRY_CODE}"
                ;;
            "moal")
                WIFI_COUNTRY_CODE_PARM="reg_alpha2=${WIFI_COUNTRY_CODE}"
                ;;
        esac
    fi

    VENDOR_WIFI_PARM=""
    case "${WIFI_MODULE}" in
        "moal")
            # NXP's driver for the Marvell 88W8987 RF SoC needs to be told what to choose between client, AP & WiFi DIRECT mode.
            VENDOR_WIFI_PARM="mod_para=nxp/wifi_mod_para_sd8987.conf"

            # And, of course, it requires a submodule...
            WIFI_DEP_MOD="mlan"
            insmod "${KMOD_PATH}/${WIFI_DEP_MOD}.ko"
            # NOTE: Nickel sleeps for two whole seconds after each module loading.
            #       Let's try our usual timing instead...
            usleep 250000
            ;;
        "wlan_drv_gen4m")
            # Nothing to do here, we only load the modules once, so we should never enter this branch.
            ;;
    esac

    WIFI_PARM=""
    if [ -n "${WIFI_COUNTRY_CODE_PARM}" ]; then
        if [ -n "${WIFI_PARM}" ]; then
            WIFI_PARM="${WIFI_PARM} ${WIFI_COUNTRY_CODE_PARM}"
        else
            WIFI_PARM="${WIFI_COUNTRY_CODE_PARM}"
        fi
    fi
    if [ -n "${VENDOR_WIFI_PARM}" ]; then
        if [ -n "${WIFI_PARM}" ]; then
            WIFI_PARM="${WIFI_PARM} ${VENDOR_WIFI_PARM}"
        else
            WIFI_PARM="${VENDOR_WIFI_PARM}"
        fi
    fi

    if [ -e "${KMOD_PATH}/${WIFI_MODULE}.ko" ]; then
        if [ -n "${WIFI_PARM}" ]; then
            # shellcheck disable=SC2086
            insmod "${KMOD_PATH}/${WIFI_MODULE}.ko" ${WIFI_PARM}
        else
            insmod "${KMOD_PATH}/${WIFI_MODULE}.ko"
        fi
    elif [ -e "/drivers/${PLATFORM}/${WIFI_MODULE}.ko" ]; then
        # NOTE: Modules are unsorted on Mk. 8
        if [ -n "${WIFI_PARM}" ]; then
            # shellcheck disable=SC2086
            insmod "/drivers/${PLATFORM}/${WIFI_MODULE}.ko" ${WIFI_PARM}
        else
            insmod "/drivers/${PLATFORM}/${WIFI_MODULE}.ko"
        fi
    fi
fi

# Race-y as hell, don't try to optimize this!
# NOTE: We're after a module insert, meaning Nickel may sleep for two whole seconds here.
case "${WIFI_MODULE}" in
    "moal")
        # NOTE: Bringup may be genuinely slower than usual with this chip, so, mimic Nickel's sleep patterns.
        sleep 2
        ;;
    *)
        sleep 1
        ;;
esac

# Bring the network interface up & setup WiFi
ifconfig "${INTERFACE}" up
[ "${WIFI_MODULE}" = "dhd" ] && wlarm_le -i "${INTERFACE}" up

pkill -0 wpa_supplicant ||
    wpa_supplicant -D "${WPA_SUPPLICANT_DRIVER}" -s -i "${INTERFACE}" -c /etc/wpa_supplicant/wpa_supplicant.conf -C /var/run/wpa_supplicant -B

