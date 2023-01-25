package com.aze.cosmiciot.classes;

import android.content.Context;
import android.content.SharedPreferences;

import java.util.Collection;
import java.util.Dictionary;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import static android.content.Context.MODE_PRIVATE;

public class Globals {

    public static Device CurrentDevice = null;

    private static String SettingsKey = "CosmicIoT";
    private static String DeviceListKey = "Devices";
    private static Set<String> DeviceList = null;

    public static boolean AddDevice(Context mCtx, String DeviceName) {
        SharedPreferences sp = mCtx.getSharedPreferences(SettingsKey, MODE_PRIVATE);
        SharedPreferences.Editor Ed=sp.edit();
        if(DeviceList == null)
        {
            //if it is null Device list not read yet, so read it
            DeviceList = GetDeviceList(mCtx);
        }
        // TODO check if device is present in DeviceList
        if(!DeviceList.contains(DeviceName))
        {
            DeviceList.add(DeviceName);
        }

        Ed.putStringSet(DeviceListKey, DeviceList );
        Ed.apply();
        return true;
    }

    public static Set<String> GetDeviceList(Context mCtx) {
        SharedPreferences sp = mCtx.getSharedPreferences(SettingsKey, MODE_PRIVATE);
        DeviceList = sp.getStringSet(DeviceListKey, new HashSet<String>());
        return DeviceList;
    }

    public static boolean IsDeviceListAvailable (Context mCtx) {
        SharedPreferences sp = mCtx.getSharedPreferences(SettingsKey, MODE_PRIVATE);
        DeviceList = sp.getStringSet(DeviceListKey, null);
        return (DeviceList!=null);
    }
}
