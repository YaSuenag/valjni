# ValJNI

ValJNI is utilities for [Valgrind](http://valgrind.org/) memcheck tool when you want to analyze your JNI / JVMTI libraries.

## How to use

I recommend to use OpenJDK with debuginfo package.

```
$ valgrind --leak-check=yes --suppressions=jvm.supp java -agentpath:libvaljni.so App
```

* `jvm.supp`
    * Valgrind suppress file.
    * You can suppress memory error in `libjvm.so` and JIT'ed code.
* `libvaljni.so`
    * JVMTI library for track [NewGlobalRef](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html#NewGlobalRef) and [DeleteGlobalRef](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html#DeleteGlobalRef) JNI functions.
    * If your JNI/JVMTI library does not release JNI global reference, it will be reported through Valgrind.

## How to build

### Requirements

* GNU Make
* GCC
* JDK
* Header files of Valgrind
    * In case of Fedora, they are provided by `valgrind-devel` package.

### Build

```
$ export JAVA_HOME=/path/to/JDK
$ make
```

or

```
$ make JAVA_HOME=/path/to/JDK
```

## License
GNU Lesser General Public License version 3
