package com.example.myapplication;


import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.viewpager.widget.ViewPager;


import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;

import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static final int REQUEST_ENABLE_BT = 10;
    int mPariedDeviceCount = 0;
    Set<BluetoothDevice> mDevieces;

    BluetoothAdapter mBluetoothAdapter;
    BluetoothDevice mRemoteDevice;
    BluetoothDevice selectedDevice;
    BluetoothSocket mSocket = null;


    static OutputStream mOutputStream = null;
    InputStream mInputStream = null;
    char mCharDelimiter = '\n';

    Thread mWorkerThread = null;
    byte[] readBuffer;
    int readBufferPosition;


    final static int BATHROOM = 1;
    final static int LUX = 2;
    final static int ENVIRONMENT = 3;
    final static int DOOR = 4;
    final static int LED = 5;

    Button btn[] = new Button[3];
    ViewPager viewPager = null;

    static Handler Chandler = null, Lhandler = null, Ehandler = null;

    Message handlermsg = null;

    boolean isBTChecked = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //viewPager
        viewPager = (ViewPager)findViewById(R.id.viewpager);
        MyViewPagerAdapter adapter = new MyViewPagerAdapter(getSupportFragmentManager());

        viewPager.setAdapter(adapter);

        btn[0] = (Button)findViewById(R.id.btn_manage);
        btn[1] = (Button)findViewById(R.id.btn_b);
        btn[2] = (Button)findViewById(R.id.btn_c);

        for(int i = 0; i < btn.length; i++){
            btn[i].setOnClickListener(this);
        }

        Chandler = new MyViewPagerAdapter.CONNECTHandler();
        Lhandler = new MyViewPagerAdapter.LEDHandler();
        Ehandler = new MyViewPagerAdapter.ENVIRONHandler();

        IntentFilter stateFilter = new IntentFilter();
        stateFilter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);
        stateFilter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
        registerReceiver(mReceiver, stateFilter);

    }

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            Log.d("Bluetooth action", action);

          //  BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

            if(action.equals(BluetoothDevice.ACTION_ACL_CONNECTED)){
                isBTChecked = true;
                btn[0].setText("관리종료");
            }
            else if(action.equals(BluetoothDevice.ACTION_ACL_DISCONNECTED)){
                isBTChecked = false;
                btn[0].setText("관리시작");

            }

        }
    };
     @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btn_manage:
                viewPager.setCurrentItem(0);
                if(isBTChecked == false) {
                    checkBluetooth();
                }
                else {
                    isBTChecked = false;
                    mBluetoothAdapter.disable();
                    btn[0].setText("관리시작");
                }
                break;
            case R.id.btn_b :
                viewPager.setCurrentItem(1);
                Toast.makeText(this, "조명 관리", Toast.LENGTH_SHORT).show();
                break;
            case R.id.btn_c :
                viewPager.setCurrentItem(2);
                Toast.makeText(this, "환경 관리", Toast.LENGTH_SHORT).show();
                break;
                default:
                    break;
        }
    }


    // 블루투스 장치의 이름이 주어졌을때 해당 블루투스 장치 객체를 페어링 된 장치 목록에서 찾아내는 코드
    BluetoothDevice getDeviceFromBondedList(String name) {
        //BluetoothDevice : 페어링 된 기기 목록을 얻어옴
        selectedDevice = null;

        //getBondedDevices 함수가 반환하는 페어링 된 기기 목록은 Set 형식이며,
        //Set 형식에서는 n 번째 원소를 얻어오는 방법이 없으므로 주어진 이름과 비교해서 찾는다.
        for (BluetoothDevice device : mDevieces) {
            //getName() : 단말기의 Bluetooth Adapter 이름을 반환
            if (name.equals(device.getName())) {
                selectedDevice = device;
                break;
            }
        }
        return selectedDevice;
    }

    //connectToSelectedDevice() : 원격 장치와 연결하는 과정을 나타냄
    //실제 데이터 송수신을 위해서는 소켓으로부터 입출력 스트림을 얻고 입출력 스트림을 이용하여 이루어진다.
    void connectToSelectedDevice(String selectedDeviceName) {
        //BluetoothDevice 원격 블투루스 기기를 나타냄
        mRemoteDevice = getDeviceFromBondedList(selectedDeviceName);

        //java.util.UUID.fromString : 자바에서 중복되지 않는 Unique 키 생성
        UUID uuid = java.util.UUID.fromString(("00001101-0000-1000-8000-00805f9b34fb"));

        try {
            //소켓 생성, RFCOMM 채널을 통한 연결
            //createRFcommSocketToServiceRecord(uuid) : 이 함수를 사용하여 원격 블루투스 장치와 통신할 수 있는 소켓을 생성함
            //이 메소드가 성공하면 스마트폰과 페어링 된 디바이스간 통신 채널에 대응하는 BluetoothSocket 오브젝트를 리턴함

            mSocket = mRemoteDevice.createRfcommSocketToServiceRecord(uuid);
            mSocket.connect(); //소켓이 생성되면 connect() 함수를 호출함으로써 두 기기의 연결은 완료된다.

            //데이터 송수신을 위한 스트림 얻기
            //BluetoothSocket 오브젝트는 두개의 Stream을 제공한다.
            //1. 데이터를 보내기 위한 OutputStream
            // 2. 데이터를 받기 위한 InputStream
            mOutputStream = mSocket.getOutputStream();
            mInputStream = mSocket.getInputStream();

            Toast.makeText(getApplicationContext(), selectedDeviceName + "와 연결되었습니다.", Toast.LENGTH_SHORT).show();
            isBTChecked = true;
            btn[0].setText("관리종료");

            //데이터 수신 준비
            beginListenForData();


        } catch (Exception e) {
            Toast.makeText(getApplicationContext(), "블루투스 연결 중 오류가 발생했습니다.", Toast.LENGTH_LONG).show();
            finish(); //App 종료
        }
    }

    // 데이터 수신 (쓰레드 사용, 수신된 메시지를 계속 검사함)
    void beginListenForData() {
        final Handler handler = new Handler();

        readBufferPosition = 0; //버퍼 내 수신 문자 저장 위치
        readBuffer = new byte[1024]; //수신 버퍼

        // 문자열 수신 쓰레드
        mWorkerThread = new Thread(new Runnable() {
            @Override
            public void run() {
                //interrupt() 메소드를 이용 쓰레드를 종료시키는 예제
                //interrupt() 메소드는 하던 일을 멈추는 메소드이다
                //isInterrupted() 메소드를 사용하여 멈추었을 경우 반복문을 나가서 스레드가 종료하게 된다.
                while (!Thread.currentThread().isInterrupted()) {
                    try {
                        //InputStream.available() : 다른 쓰레드에서 blocking 하기 전까지 읽을 수 있는 문자열의 개수를 반환
                        int byteAvailable = mInputStream.available(); //수신 데이터 확인
                        if (byteAvailable > 0) { //데이터가 수신된 경우

                            byte[] packetBytes = new byte[byteAvailable];
                            //read(buf[]) : 입력 스트림에서 buf[]  크기만큼 읽어서 저장. 없을 경우에 -1 리턴
                            mInputStream.read(packetBytes);
                            for (int i = 0; i < byteAvailable; i++) {
                                byte b = packetBytes[i];
                                if (b == mCharDelimiter) { //문자열 마지막 \n 이면
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    //System.arraycopy(복사할 배열, 복사 시작점, 복사된 배열, 붙이기 시작점, 복사할 개수)
                                    //readBuffer 배열을 처음부터 끝까지 encodedBytes 배열로 복사
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);

                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    handler.post(new Runnable() {
                                        // 수신된 문자열 데이터에 대한 처리
                                        String msg = "";
                                        @Override
                                        public void run() {
                                            String[] arr = data.split(" : ");
                                            if(arr[0].equals("led")){

                                                handlermsg = Lhandler.obtainMessage(LED, arr[1]);
                                                Lhandler.sendMessage(handlermsg);
                                            }
                                            else if(arr[0].equals("env")) {

                                                handlermsg = Ehandler.obtainMessage(ENVIRONMENT, arr[1]);
                                                Ehandler.sendMessage(handlermsg);
                                            }
                                            else if(arr[0].equals("lux")){
                                                handlermsg = Lhandler.obtainMessage(LUX, arr[1]);
                                                Lhandler.sendMessage(handlermsg);
                                            }
                                            else if(arr[0].equals("bathroom")){
                                                handlermsg = Lhandler.obtainMessage(BATHROOM, arr[1]);
                                                Lhandler.sendMessage(handlermsg);
                                            }
                                            else if(arr[0].equals("door")) {
                                                handlermsg = Ehandler.obtainMessage(DOOR, arr[1]);
                                                Ehandler.sendMessage(handlermsg);
                                            }
                                        }
                                    });
                                } else { //문자열 마지막이 아니면 buffer에 b넣기
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }


                    } catch (Exception e) { //데이터 수신 중 오류 발생
                        beginListenForData();
                        Toast.makeText(getApplicationContext(), "데이터 수신 중 오류가 발생했습니다.", Toast.LENGTH_LONG).show();
                       // finish(); //App 종료
                    }
                }
            }
        });
        mWorkerThread.start();
    }


    //블루투스 지원하며 활성 상태인 경우
    void selectDevice() {
        // 블루투스 디바이스는 연결해서 사용하기 전에 먼저 페어링 되어야함
        // getBoundedDevices() : 페어링 된 장치 목록 얻어오는 함수
        mDevieces = mBluetoothAdapter.getBondedDevices();
        mPariedDeviceCount = mDevieces.size();

        if (mPariedDeviceCount == 0) { // 페어링된 장치가 없는 경우
            Toast.makeText(getApplicationContext(), "페어링된 장치가 없습니다.", Toast.LENGTH_LONG).show();
            finish(); //App 종료
        }

        //페어링 된 장치가 있는 경우
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("블루투스 장치 선택");

        //각 디바이스는 이름과 (서로 다른)주소를 가진다. 페어링 된 디바이스들을 표시한다.
        List<String> listItems = new ArrayList<String>();
        for (BluetoothDevice device : mDevieces) {
            //device.getName() : 단말기의 Bluetooth Adapter 이름을 반환
            listItems.add(device.getName());
        }
        listItems.add("취소"); // 취소 항목 추가

        //CharSequence : 변경 가능한 문자열
        //toArray : List 형태로 넘어온 것 배열로 바꿔서 처리하기 위한 toArray()함수
        final CharSequence[] items = listItems.toArray(new CharSequence[listItems.size()]);
        //toArray 함수를 이용해서 size 만큼 배열이 생성되었다.
        //listItems.toArray(new CharSequence[listItems.size()]);

        builder.setItems(items, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int item) {
                if (item == mPariedDeviceCount) { //연결 할 장치를 선택하지 않고 '취소'를 누른 경우
                    Toast.makeText(getApplicationContext(), "연결할 장치를 선택하지 않았습니다.", Toast.LENGTH_LONG).show();
                    finish();
                } else { //연결할 장치를 선택한 경우, 선택한 장치와 연결을 시도함
                    connectToSelectedDevice(items[item].toString());
                }
            }
        });

        builder.setCancelable(false); //뒤로 가기 버튼 사용 금지
        AlertDialog alert = builder.create();
        alert.show();
    }

    void checkBluetooth() {
        /*
        getDefaultAdapter() : 만일 폰에 블루투스 모듈이 없으면 null을 리턴
                                이 경우 Toast를 사용해 에러 메시지를 표시하고 앱을 종료한다
         */
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) { //블루투스 미지원
            Toast.makeText(getApplicationContext(), "기기가 블루투스를 지원하지 않습니다.", Toast.LENGTH_LONG).show();
            finish();
        } else { // 블루투스 지원
            /*
            isEnable() : 블루투스 모듈이 활성화 되었는지 확인
                        true : 지원, false : 미지원
             */
            if (!mBluetoothAdapter.isEnabled()) { // 기기가 블루투스를 지원하지만 비활성 상태인 경우
                Toast.makeText(getApplicationContext(), "현재 블루투스가 비활성 상태입니다.", Toast.LENGTH_LONG).show();
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                //REQUEST_ENABLE_BT : 블루투스 활성 상태의 변경 결과를 App으로 알려줄 때 식별자로 사용(0이상)
                /*
                startActivityForResult 함수 호출 후 다이얼로그가 나타남
                "예"를 선택하면 시스템의 블루투스 장치를 활성화 시키고
                "아니오"를 선택하면 비활성화 상태를 유지한다.
                선택 결과는 onActivityResult 콜백 함수에서 확인할 수 있다.
                 */
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            } else //블루투스를 지원하며 활성 상태인 경우
                selectDevice();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
//        unregisterReceiver(mReceiver);
    }


    // onDestroy() : 어플이 종료될때 호출되는 함수
    // 블루투스 연결이 필요하지 않는 경우 입출력 스트림 소켓을 닫아줌

    @Override
    public void onDestroy() {
        try {
            mWorkerThread.interrupt(); // 데이터 수신 쓰레드 종료
            mInputStream.close();
            mSocket.close();
            unregisterReceiver(mReceiver);
        } catch (Exception e) {
        }
        super.onDestroy();
    }

    // onActivityResult : 사용자의 선택결과 확인 (아니오, 예)
    // RESULT_OK : 블루투스가 활성화 상태로 변경된 경우, "예"
    // RESULT_CANCELED : 오류나 사용자의 "아닌오" 선택으로 비활성 상태로 남아 있는 경우

    /*
    사용자가 request 를 허가 (또는 거부)하면 안드로이드 앱의 onActivityResult 메소드를 호출해서 request의 허가/거부를 확인할 수 있따.
     requestCode : startActivityForResult 에서 아용했던 요청 코드. REQUEST_ENABLE_BT 값
     resultCode : 종료된 액티비티가 setResult로 지정한 결과 코드. RESULT_OK, RESULT_CANCELED 값 중 하나가 들어감
     data : 종료된 액티비티가 인텐트를 첨부했을 경우 그 인텐트가 들어있고, 첨부하지 않으면 null
     */

    @Override
    public void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        // startActivityForResult 를 여러번 사용할 땐 이런 식으로 switch 문을 사용하여 어떤 요청인지 구분하여 사용함
        switch (requestCode) {
            case REQUEST_ENABLE_BT:
                if (resultCode == RESULT_OK) { //블루투스 활성화 상태
                    selectDevice();
                } else if (resultCode == RESULT_CANCELED) { //블루투스 비활성화 상태(종료)
                    Toast.makeText(getApplicationContext(), "블루투스를 사용할 수 없어 프로그램을 종료합니다.", Toast.LENGTH_LONG).show();
                    finish();
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }


}
