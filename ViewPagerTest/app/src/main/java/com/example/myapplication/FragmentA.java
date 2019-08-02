package com.example.myapplication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;


public class FragmentA extends Fragment {
    final static int CONNECT_SUCCESS = 1;
    final static int DISCONNECTED = 2;

    Button btnConnect;

    static  Button BtnConnect;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragmenta, container, false);


        btnConnect = (Button)view.findViewById(R.id.btn_connect);
        BtnConnect = btnConnect;
        btnConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if((BtnConnect.getText()).equals("연결"))
                    ((MainActivity)getActivity()).checkBluetooth();
                else {
                    ((MainActivity)getContext()).mBluetoothAdapter.disable();
                }
            }
        });

        return view;

    }

    public static class HandlerA extends Handler {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            if(msg.what == CONNECT_SUCCESS){
                BtnConnect.setText("해제");
            }
            else if(msg.what == DISCONNECTED) {
                BtnConnect.setText("연결");
            }
            //System.out.println(msg.obj);

            // Toast.makeText(this, msg.what, Toast.LENGTH_SHORT).show();

        }
    }
}
