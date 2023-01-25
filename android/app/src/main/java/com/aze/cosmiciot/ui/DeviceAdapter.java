package com.aze.cosmiciot.ui;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.aze.cosmiciot.R;
import com.aze.cosmiciot.classes.Device;

import java.util.List;

import androidx.annotation.NonNull;
import androidx.cardview.widget.CardView;
import androidx.recyclerview.widget.RecyclerView;

public class DeviceAdapter extends RecyclerView.Adapter <DeviceAdapter.DeviceViewHolder> {


    //this context we will use to inflate the layout
    private Context mCtx;

    //we are storing all the Web Enquiries in a list
    private List<Device> deviceList;

    //to listen the on click events of the web enquiry
    private DeviceAdapterListener listener;

    public DeviceAdapter(Context mCtx, List<Device> deviceList, DeviceAdapterListener listener) {
        this.mCtx = mCtx;
        this.deviceList = deviceList;
//        this.deviceListFiltered = deviceList;
        this.listener = listener;
    }

    @NonNull
    @Override
    public DeviceViewHolder onCreateViewHolder(@NonNull  ViewGroup viewGroup, int i) {
        //inflating and returning our view holder
        LayoutInflater inflater = LayoutInflater.from(mCtx);
        View view = inflater.inflate(R.layout.cardview, null);
        return new DeviceViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull DeviceViewHolder deviceViewHolder, int i) {

        Device device = deviceList.get(i);

        deviceViewHolder.textViewTitle.setText(device.Name);
        deviceViewHolder.imageViewThumbNail.setImageResource(R.drawable.farm);

        // Apply On Click Events
        applyClickEvents(deviceViewHolder, device);
    }

    private void applyClickEvents(DeviceViewHolder holder, final Device device) {

        holder.cardView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listener.onDeviceClicked(device);
            }
        });
    }
    @Override
    public int getItemCount() {
        return deviceList.size();
    }

    class DeviceViewHolder extends RecyclerView.ViewHolder {

        TextView textViewTitle;
        ImageView imageViewThumbNail;
        CardView cardView;

        public DeviceViewHolder(View itemView) {
            super(itemView);

            textViewTitle = itemView.findViewById(R.id.title);
            imageViewThumbNail = itemView.findViewById(R.id.thumbnail);
            cardView = itemView.findViewById(R.id.PCardView);
        }

    }

    public interface DeviceAdapterListener {
        void onDeviceClicked(Device device);
    }
}
