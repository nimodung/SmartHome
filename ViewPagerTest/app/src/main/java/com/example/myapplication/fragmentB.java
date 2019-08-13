package com.example.myapplication;


import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import java.io.IOException;


public class fragmentB extends Fragment{


    Button btnLedRoom, btnLedLivingroom, btnLedKitchen, btnLedBathroom, btnLedDoor, btnLedWhole;
    TextView tvLedRoom, tvLedLivingroom, tvLedKitchen, tvLedBathroom, tvLedDoor, tvLedWhole;
    Switch swBathRoom, swDoor;

    static boolean roomLedState = false, kitchenLedState = false, livingroomLedState = false, wholeLedState = false;
    static boolean  bathroomLedState = false, doorLedState = false;
    static boolean doorLedAutoState = true, bathroomLedAutoState = true;

    String mStrDelimiter = "\n";
    String msg = "";

    final static int BATHROOM = 1;
    final static int LUX = 2;
    final static int LED = 5;

    static View view;
    static Button btnDOOR, btnBATHROOM;
    static TextView tvROOM, tvLIVINGR, tvKitchen, tvBATHR, tvDOOR, tvWHOLE;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragmentb, container, false);

        btnLedRoom = (Button)view.findViewById(R.id.btn_room);
        tvLedRoom = (TextView)view.findViewById(R.id.tv_room);
        btnLedKitchen = (Button)view.findViewById(R.id.btn_kitchen);
        tvLedKitchen = (TextView)view.findViewById(R.id.tv_kitchen);
        btnLedLivingroom = (Button)view.findViewById(R.id.btn_livingroom);
        tvLedLivingroom = (TextView)view.findViewById(R.id.tv_livingroom);
        btnLedBathroom = (Button)view.findViewById(R.id.btn_bathroom);
        tvLedBathroom = (TextView)view.findViewById(R.id.tv_bathroom);
        btnLedDoor = (Button)view.findViewById(R.id.btn_door);
        tvLedDoor = (TextView)view.findViewById(R.id.tv_door);
        btnLedWhole = (Button)view.findViewById(R.id.btn_whole);
        tvLedWhole = (TextView)view.findViewById(R.id.tv_whole);

        swBathRoom = (Switch)view.findViewById(R.id.sw_bathroom);
        swDoor = (Switch)view.findViewById(R.id.sw_door);

        btnDOOR = btnLedDoor;
        btnBATHROOM = btnLedBathroom;

        tvROOM = tvLedRoom;
        tvLIVINGR = tvLedLivingroom;
        tvKitchen = tvLedKitchen;
        tvBATHR = tvLedBathroom;
        tvDOOR = tvLedDoor;
        tvWHOLE = tvLedWhole;

        if(roomLedState) {
            tvLedRoom.setText("ON");
        }
        else {
            tvLedRoom.setText("OFF");
        }
        if(kitchenLedState) {
            tvLedKitchen.setText("ON");
        }
        else {
            tvLedKitchen.setText("OFF");
        }
        if(livingroomLedState) {
            tvLedLivingroom.setText("ON");
        }
        else {
            tvLedLivingroom.setText("OFF");
        }
        if(bathroomLedState) {
            tvLedBathroom.setText("ON");
        }
        else {
            tvLedBathroom.setText("OFF");
        }
        if(doorLedState) {
            tvLedDoor.setText("ON");
        }
        else {
            tvLedDoor.setText("OFF");
        }
        if(wholeLedState) {
            tvLedWhole.setText("ON");
        }
        else {
            tvLedWhole.setText("OFF");
        }
      //  btnLedDoor.setOnClickListener(LedControl);
        return view;
    }


    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        btnLedRoom.setOnClickListener(LedControl);
        btnLedKitchen.setOnClickListener(LedControl);
        btnLedLivingroom.setOnClickListener(LedControl);
        btnLedBathroom.setOnClickListener(LedControl);
        btnLedDoor.setOnClickListener(LedControl);
        btnLedWhole.setOnClickListener(LedControl);


        swBathRoom.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                try {
                    String stateMSG = "";
                    if(isChecked){
                        bathroomLedAutoState = true;
                        stateMSG= "bathroomLedAutoStatetrue\n";
                        btnLedBathroom.setEnabled(false);
                    }
                    else {
                        bathroomLedAutoState = false;
                        stateMSG= "bathroomLedAutoStatefalse\n";
                        btnLedBathroom.setEnabled(true);
                    }


                    ((MainActivity)getContext()).mOutputStream.write(stateMSG.getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        swDoor.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                try {
                    String stateMSG = "";
                    if(isChecked){
                        doorLedAutoState = true;
                        stateMSG = "doorLedAutoStatetrue" + mStrDelimiter;
                        btnLedDoor.setEnabled(false);
                    }
                    else {
                        doorLedAutoState = false;
                        stateMSG = "doorLedAutoStatefalse" + mStrDelimiter;
                        btnLedDoor.setEnabled(true);}


                    ((MainActivity)getContext()).mOutputStream.write(stateMSG.getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

    }


    View.OnClickListener LedControl = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            try {
                switch (view.getId()) {
                    case R.id.btn_room:
                        if (roomLedState) {msg = "led room off" + mStrDelimiter;}
                        else if (!roomLedState) { msg = "led room on" + mStrDelimiter;}
                        break;
                    case R.id.btn_kitchen :
                        if (kitchenLedState) {msg = "led kitchen off" + mStrDelimiter;}
                        else if (!kitchenLedState) {msg = "led kitchen on" + mStrDelimiter;}
                        break;
                    case R.id.btn_livingroom :
                        if (livingroomLedState) {msg = "led living off" + mStrDelimiter;}
                        else if (!livingroomLedState) {msg = "led living on" + mStrDelimiter;}
                        break;
                    case R.id.btn_bathroom :
                        if (bathroomLedState) {msg = "led bathroom off" + mStrDelimiter;}
                        else if (!bathroomLedState) {msg = "led bathroom on" + mStrDelimiter;}
                        break;
                    case R.id.btn_door :
                        if (doorLedState) { msg = "led door off" + mStrDelimiter; }
                        else if (!doorLedState) {msg = "led door on" + mStrDelimiter; }
                        break;
                    case R.id.btn_whole :
                        if(!doorLedAutoState && !bathroomLedAutoState) {
                            if (wholeLedState) {msg = "led whole off" + mStrDelimiter;}
                            else if (!wholeLedState) {msg = "led whole on" + mStrDelimiter;}
                        }
                        if(doorLedAutoState && bathroomLedAutoState) {
                            if (wholeLedState) {msg = "led whole off auto" + mStrDelimiter;}
                            else if (!wholeLedState) {msg = "led whole on auto" + mStrDelimiter;}
                        }
                        else if(doorLedAutoState && !bathroomLedAutoState) {
                            if(wholeLedState) {msg = "led whole off door" + mStrDelimiter;}
                            else{msg = "led whole on door" + mStrDelimiter; }
                        }
                        else if(!doorLedAutoState && bathroomLedAutoState) {
                            if(wholeLedState) {msg = "led whole off bathroom" + mStrDelimiter;}
                            else{msg = "led whole on bathroom" + mStrDelimiter;}
                        }
                        break;
                }
                ((MainActivity)getContext()).mOutputStream.write(msg.getBytes());
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    static  void wholeSetText(String str) {
        tvWHOLE.setText(str); tvROOM.setText(str); tvKitchen.setText(str);
        tvLIVINGR.setText(str);
        if(!doorLedAutoState) tvDOOR.setText(str);
        if(!bathroomLedAutoState) tvBATHR.setText(str);
    }

    public static class HandlerB extends Handler {

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            if(view != null) {

                if(msg.what == LUX) {
                    String strMsg = msg.obj.toString();
                    if(strMsg.equals("on")) {
                        doorLedState = false;
                    }
                    else if(strMsg.equals("off")){
                        doorLedState = true;
                    }
                    btnDOOR.performClick();
                }
                if(msg.what == BATHROOM){
                    String strMsg = msg.obj.toString();
                    if(strMsg.equals("on")) {
                        bathroomLedState = false;
                    }
                    else if(strMsg.equals("off")){
                        bathroomLedState = true;
                    }
                    btnBATHROOM.performClick();
                }
                if(msg.what == LED){
                    String[] strMsg = (msg.obj.toString()).trim().split(" ");


                    if(strMsg[0].equals("room")){
                        if(strMsg[1].equals("on")){
                            tvROOM.setText("ON");
                            roomLedState = true;
                        }
                        else if(strMsg[1].equals("off")){
                            tvROOM.setText("OFF");
                            roomLedState = false;
                        }
                    }
                    else if(strMsg[0].equals("living")){
                        if(strMsg[1].equals("on")){
                            tvLIVINGR.setText("ON");
                            livingroomLedState = true;
                        }
                        else if(strMsg[1].equals("off")){
                            tvLIVINGR.setText("OFF");
                            livingroomLedState = false;
                        }
                    }
                    else if(strMsg[0].equals("kitchen")){
                        if(strMsg[1].equals("on")){
                            tvKitchen.setText("ON");
                            kitchenLedState = true;
                        }
                        else if(strMsg[1].equals("off")){
                            tvKitchen.setText("OFF");
                            kitchenLedState = false;
                        }
                    }
                    else if(strMsg[0].equals("bathroom")){
                        if(strMsg[1].equals("on")){
                            tvBATHR.setText("ON");
                            bathroomLedState = true;
                        }
                        else if(strMsg[1].equals("off")){
                            tvBATHR.setText("OFF");
                            bathroomLedState = false;
                        }
                    }
                    else if(strMsg[0].equals("door")){
                        if(strMsg[1].equals("on")){
                            tvDOOR.setText("ON");
                            doorLedState = true;
                        }
                        else if(strMsg[1].equals("off")){
                            tvDOOR.setText("OFF");
                            doorLedState = false;
                        }
                    }
                    else if(strMsg[0].equals("whole") || strMsg[0].equals("wholed") || strMsg[0].equals("wholeb")){
                        if(strMsg[1].equals("on")){
                            wholeSetText("ON");
                            wholeLedState = true;
                        }
                        else if(strMsg[1].equals("off")){
                            wholeSetText("OFF");
                            wholeLedState = false;
                        }
                    }
                }
            }


        }
    }
}
