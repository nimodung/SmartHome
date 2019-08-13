package com.example.myapplication;


import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import java.io.IOException;


public class FragmentC extends Fragment implements View.OnClickListener {

    TextView tvTemper, tvHumi, tvLux, tvCooler, tvDoorOpen, tvWindowOpen;
    Button btnCooler, btnDoorOpen, btnWindowOpen;

    final static int ENVIRONMENT = 3;
    final static int DOOR = 4;

    static TextView Temper, Humi, Lux, Door, Window;
    static View view;

    String mStrDelimiter = "\n";


    boolean coolerSatate = false;
    static boolean doorState = false, windowState = false;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragmentc, container, false);

        tvTemper = (TextView)view.findViewById(R.id.tv_temper);
        tvHumi = (TextView)view.findViewById(R.id.tv_humi);
        tvLux = (TextView)view.findViewById(R.id.tv_lux);

        btnCooler = (Button)view.findViewById(R.id.btn_cooler);
        tvCooler = (TextView)view.findViewById(R.id.tv_cooler);
        btnCooler.setOnClickListener(this);

        btnDoorOpen = (Button)view.findViewById(R.id.btn_dooropen);
        tvDoorOpen = (TextView)view.findViewById(R.id.tv_dooropen);
        btnDoorOpen.setOnClickListener(this);

        btnWindowOpen = (Button)view.findViewById(R.id.btn_window);
        tvWindowOpen = (TextView)view.findViewById(R.id.tv_window);
        btnWindowOpen.setOnClickListener(this);

        Temper = tvTemper;
        Humi = tvHumi;
        Lux = tvLux;
        Door = tvDoorOpen;
        Window = tvWindowOpen;

        return view;
    }



    public static class HandlerC extends Handler {

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            if(view != null) {


                if (msg.what == ENVIRONMENT) {

                    String[] tempMsg = (msg.obj.toString()).trim().split(" ");
                    if (tempMsg[0].equals("humi")) {
                        Humi.setText(tempMsg[1]);
                    }
                    if (tempMsg[2].equals("temper")) {
                        Temper.setText(tempMsg[3]);
                    }
                    if(tempMsg[4].equals("lux")){
                        Lux.setText(tempMsg[5]);
                    }
                }
                else if(msg.what == DOOR){
                    if(((msg.obj).toString()).equals("closed")) {
                        doorState = false;
                        Door.setText("CLOSE");
                    }
                }
            }
        }
    }

    @Override
    public void onClick(View view) {
        try {
            String outMsg = "";
            switch (view.getId()) {
                case R.id.btn_cooler:
                    if(!coolerSatate){
                        outMsg = "fan on" + mStrDelimiter;
                        tvCooler.setText("ON");
                        coolerSatate = true;
                    }
                    else {
                        outMsg = "fan off" + mStrDelimiter;
                        tvCooler.setText("OFF");
                        coolerSatate = false;
                    }

                    break;
                case R.id.btn_dooropen:
                    if(!doorState) {
                        outMsg = "door open" + mStrDelimiter;
                        tvDoorOpen.setText("OPEN");
                        doorState = true;
                    }
                    else {
                        outMsg = "door close" + mStrDelimiter;
                        tvDoorOpen.setText("CLOSE");
                        doorState = false;
                    }
                    break;
                case R.id.btn_window:
                    if(!windowState){
                        outMsg = "window open" + mStrDelimiter;
                        tvWindowOpen.setText("OPEN");
                        windowState = true;
                    }
                    else {
                        outMsg = "window close" + mStrDelimiter;
                        tvWindowOpen.setText("CLOSE");
                        windowState = false;
                    }
            }

            ((MainActivity)getContext()).mOutputStream.write(outMsg.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
