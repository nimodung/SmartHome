package com.example.myapplication;

import android.content.Context;
import android.net.Uri;
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
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;


public class fragmentB extends Fragment {

    final static int ROOM = 3;
    final static int LIVINGROOM = 4;
    final static int KITCHEN = 5;
    final static int BATHROOM = 6;
    final static int DOOR = 7;
    final static int WHOLE = 8;

    Button btnLedRoom, btnLedLivingroom, btnLedKitchen, btnLedBathroom, btnLedDoor, btnLedWhole;
    TextView tvLedRoom, tvLedLivingroom, tvLedKitchen, tvLedBathroom, tvLedDoor, tvLedWhole;

    boolean roomLedState = false, kitchenLedState = false, livingroomLedState = false,
            bathroomLedState = false, doorLedState = false, wholeLedState = false;
    boolean doorLedAutoState = true, bathroomLedAutoState = false;

    String mStrDelimiter = "\n";
    String msg = "";

    MainActivity context;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragmentb, container, false);

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

        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        btnLedRoom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LedControl(v);
            }
        });

        btnLedKitchen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LedControl(v);
            }
        });

        btnLedLivingroom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LedControl(v);
            }
        });

        btnLedBathroom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LedControl(v);
            }
        });

        btnLedDoor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LedControl(v);
            }
        });

        btnLedWhole.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LedControl(v);
            }
        });
    }

    void LedControl(View v) {
        try {
            switch (v.getId()) {
                case R.id.btn_room:
                    if (roomLedState) {
                        msg = "led room off" + mStrDelimiter;
                        tvLedRoom.setText("OFF");
                        roomLedState = false;
                    } else if (!roomLedState) {
                        msg = "led room on" + mStrDelimiter;
                        tvLedRoom.setText("ON");
                        roomLedState = true;
                    }
                    break;
                case R.id.btn_kitchen :
                    if (kitchenLedState) {
                        msg = "led kitchen off" + mStrDelimiter;
                        tvLedKitchen.setText("OFF");
                        kitchenLedState = false;
                    } else if (!kitchenLedState) {
                        msg = "led kitchen on" + mStrDelimiter;
                        tvLedKitchen.setText("ON");
                        kitchenLedState = true;
                    }
                    break;
                case R.id.btn_livingroom :
                    if (livingroomLedState) {
                        msg = "led living off" + mStrDelimiter;
                        tvLedLivingroom.setText("OFF");
                        livingroomLedState = false;
                    } else if (!livingroomLedState) {
                        msg = "led living on" + mStrDelimiter;
                        tvLedLivingroom.setText("ON");
                        livingroomLedState = true;
                    }
                    break;
                case R.id.btn_bathroom :
                    if (bathroomLedState) {
                        msg = "led bathroom off" + mStrDelimiter;
                        tvLedBathroom.setText("OFF");
                        bathroomLedState = false;
                    } else if (!bathroomLedState) {
                        msg = "led bathroom on" + mStrDelimiter;
                        tvLedBathroom.setText("ON");
                        bathroomLedState = true;
                    }
                    break;
                case R.id.btn_door :
                    if(!doorLedAutoState) {
                        if (doorLedState) {
                            msg = "led door off" + mStrDelimiter;
                            tvLedDoor.setText("OFF");
                            doorLedState = false;
                        } else if (!doorLedState) {
                            msg = "led door on" + mStrDelimiter;
                            tvLedDoor.setText("ON");
                            doorLedState = true;
                        }
                    }
                    break;
                case R.id.btn_whole :
                    if(!doorLedAutoState && !bathroomLedAutoState) {
                        if (wholeLedState) {
                            msg = "led whole off" + mStrDelimiter;
                            tvLedWhole.setText("OFF");
                            wholeLedState = false;
                        } else if (!wholeLedState) {
                            msg = "led whole on" + mStrDelimiter;
                            tvLedWhole.setText("ON");
                            wholeLedState = true;
                        }
                    }
                    else if(doorLedAutoState && !bathroomLedAutoState) {
                        if(wholeLedState) {
                            msg = "led whole off door" + mStrDelimiter;
                            wholeSetText("OFF");
                            wholeLedState = false;
                        }
                        else{
                            msg = "led whole on door" + mStrDelimiter;
                            wholeSetText("ON");
                            wholeLedState = true;
                        }
                    }
                    break;
            }
            ((MainActivity)getContext()).mOutputStream.write(msg.getBytes());
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    void wholeSetText(String str) {
        tvLedWhole.setText(str); tvLedRoom.setText(str); tvLedKitchen.setText(str);
        tvLedLivingroom.setText(str);
        if(!doorLedAutoState) tvLedDoor.setText(str);
        if(!bathroomLedAutoState) tvLedBathroom.setText(str);
    }

    public static class HandlerB extends Handler {

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);


            //System.out.println(msg.obj);

           // Toast.makeText(this, msg.what, Toast.LENGTH_SHORT).show();

        }
    }
}
