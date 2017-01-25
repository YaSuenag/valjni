public class GlobalRefLeak{

  private static native void leakJNIGlobalRef(Object obj);

  static{
    System.loadLibrary("jni-sample");
  }

  public static void main(String[] args){
    leakJNIGlobalRef(new Object());
  }

}
