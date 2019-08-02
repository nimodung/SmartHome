package com.example.myapplication;

import android.os.Handler;
import android.os.Message;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentStatePagerAdapter;


public class MyViewPagerAdapter extends FragmentStatePagerAdapter {
    /*
        이 클래스의 부모 생성자 호출시 인수로 반드시 FragmentManager 객체를 넘겨야 한다.
        이 객체는 Activity에서만 만들 수 있고, 여기서 사용중인 Fragment가 v4이기 때문에
        Activity 중에서도 ActionBarActivity에서 얻어와야 한다.
     */

    static Handler LEDHandler = null;
    static Handler CONNECTHandler = null;
    Fragment[] fragments = new Fragment[3];


    public MyViewPagerAdapter(FragmentManager fm) {
        super(fm);
        fragments[0] = new FragmentA();
        fragments[1] = new fragmentB();
        fragments[2] = new FragmentC();
        CONNECTHandler = new FragmentA.HandlerA();
        LEDHandler = new fragmentB.HandlerB();
    }

    // 아래의 메서드들의 호출 주체는 ViewPager 이다.
    // ListView 와 원리가 같다.

    // 여러 프래그먼트 중 어떤 프레그먼트를 보여줄 지 결정
    public Fragment getItem(int arg0) {
        return fragments[arg0];
    }

    // 보여질 프레그 먼트가 몇개인지 결정
    public int getCount() {
        return fragments.length;
    }

    public static class LEDHandler extends Handler {
        Message adapterMsg = null;
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            adapterMsg = LEDHandler.obtainMessage(msg.what, msg.obj);
          //  System.out.println(msg.obj);
            LEDHandler.sendMessage(adapterMsg);
        }
    }

    public static class CONNECTHandler extends Handler {
        Message adapterMsg = null;
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            adapterMsg = CONNECTHandler.obtainMessage(msg.what, msg.obj);
            //  System.out.println(msg.obj);
            CONNECTHandler.sendMessage(adapterMsg);
        }
    }
}
