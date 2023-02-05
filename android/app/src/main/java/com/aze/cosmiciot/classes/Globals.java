package com.aze.cosmiciot.classes;

import android.content.Context;
import android.content.SharedPreferences;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
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

    private static String File_Name = "config.json";

    private static String DeviceListKey = "devicelist";

    private static Set<String> DeviceList = null;

    public static boolean AddDevice(Context mCtx, String device) {
        if(DeviceList == null)
        {
            //if it is null Device list not read yet, so read it
            DeviceList = GetDeviceList(mCtx);
        }

        //if Device list "null" no content in the list, so create new device
        if(DeviceList == null)
        {
            DeviceList = new HashSet<String>();
        }

        if(!DeviceList.contains(device))
        {
            DeviceList.add(device);
            SaveSettingsFile(mCtx);
        }

        return true;
    }

    public static boolean RemoveDevice(Context mCtx, String device) {
        if(DeviceList == null)
        {
            //if it is null Device list not read yet, so read it
            DeviceList = GetDeviceList(mCtx);
        }

        //if Device list "null" no content in the list, so create new device
        if(DeviceList != null)
        {
            if(DeviceList.contains(device))
            {
                DeviceList.remove(device);
                SaveSettingsFile(mCtx);
            }
        }
        return true;
    }

    public static Set<String> GetDeviceList(Context mCtx) {
        String ConfigData = readFromFile(mCtx, File_Name);
        if(ConfigData != null){

            Gson gson = new Gson();
            JsonObject jobject = gson.fromJson(ConfigData, JsonObject.class);

            JsonArray jarray = jobject.getAsJsonArray(DeviceListKey);
            DeviceList = new HashSet<String>();
            for(int i=0;i< jarray.size();i++){
                DeviceList.add(jarray.get(i).getAsString());
            }
        }
        else {
            writeToFile( mCtx, File_Name, "{}" );
        }
        return DeviceList;
    }

    public static boolean IsDeviceListAvailable (Context mCtx) {
        String ConfigData = readFromFile(mCtx, File_Name);

        if(ConfigData != null){
            return true;
        }
        return false;
    }

    private static void SaveSettingsFile(Context mCtx){

        JsonArray deviceList = new JsonArray();
        for(String deviceName : DeviceList) {
            deviceList.add(deviceName);
        }

        JsonObject obj = new JsonObject();
        obj.add(DeviceListKey, deviceList);

        writeToFile(mCtx,File_Name,obj.toString());
    }

    private static String readFromFile(Context context, String fileName) {

        String ret = null;

        try {
            InputStream inputStream = context.openFileInput(fileName);

            if ( inputStream != null ) {
                InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
                BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
                String receiveString = "";
                StringBuilder stringBuilder = new StringBuilder();

                while ( (receiveString = bufferedReader.readLine()) != null ) {
                    stringBuilder.append("\n").append(receiveString);
                }

                inputStream.close();
                ret = stringBuilder.toString();
            }
        }
        catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return ret;
    }

    private static void writeToFile(Context context, String fileName, String data) {
        try {
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(context.openFileOutput(fileName, Context.MODE_PRIVATE));
            outputStreamWriter.write(data);
            outputStreamWriter.close();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

}
