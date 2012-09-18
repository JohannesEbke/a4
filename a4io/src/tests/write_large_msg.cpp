#include <iostream>

#include <a4/output_stream.h>
#include <a4/io/Tests.pb.h>

using namespace std;
using namespace a4::io;

int main(int argc, char ** argv) {
    int seed = 42;
    std::string fn;
    if (argc == 1) {
        fn = "test_large_msg.a4";
    } else if (argc == 2) {
        fn = argv[1];
    } else if (argc == 3) {
        fn = argv[1];
        seed = atoi(argv[2]);
    } else assert(argc <= 3);
    srandom(seed);
    std::cout << "Seed is " << seed << std::endl;

    OutputStream w(fn, "TestEvent");
    w.set_compression("UNCOMPRESSED");
    w.set_forward_metadata();

    TestMetaData m;
    m.set_meta_data(seed);
    w.metadata(m);
    const int N = 1000;
    const int M = 100;
    for(int i = 0; i < N; i++) {
        TestEvent e;
        e.set_event_number(seed*1000 + i);
#define FILL_OBJECTS(WHICH, NUMBER) \
        for(int j = 0; j < NUMBER; j++) { \
            TestObject * o = e.add_objects ## WHICH();\
            o->set_vx(random()*100.0/RAND_MAX);\
            o->set_vy(random()*100.0/RAND_MAX);\
            o->set_vz(random()*100.0/RAND_MAX);\
            o->set_vt(random()*100.0/RAND_MAX);\
            o->set_d1(random()*100.0/RAND_MAX);\
            o->set_d2(random()*100.0/RAND_MAX);\
            o->set_d3(random()*100.0/RAND_MAX);\
            o->set_d4(random()*100.0/RAND_MAX);\
            o->set_d5(random()*100.0/RAND_MAX);\
            o->set_d6(random()*100.0/RAND_MAX);\
        }
        FILL_OBJECTS(00, M);
        FILL_OBJECTS(01, M);
        FILL_OBJECTS(02, M);
        FILL_OBJECTS(03, M);
        FILL_OBJECTS(04, M);
        FILL_OBJECTS(05, M);
        FILL_OBJECTS(06, M);
        FILL_OBJECTS(07, M);
        FILL_OBJECTS(08, M);
        FILL_OBJECTS(09, M);
        FILL_OBJECTS(10, M);
        FILL_OBJECTS(11, M);
        FILL_OBJECTS(12, M);
        FILL_OBJECTS(13, M);
        FILL_OBJECTS(14, M);
        FILL_OBJECTS(15, M);
        FILL_OBJECTS(16, M);
        FILL_OBJECTS(17, M);
        FILL_OBJECTS(18, M);
        FILL_OBJECTS(19, M);
        FILL_OBJECTS(20, M);
        FILL_OBJECTS(21, M);
        FILL_OBJECTS(22, M);
        FILL_OBJECTS(23, M);
        FILL_OBJECTS(24, M);
        FILL_OBJECTS(25, M);
        FILL_OBJECTS(26, M);
        FILL_OBJECTS(27, M);
        FILL_OBJECTS(28, M);
        FILL_OBJECTS(29, M);
        FILL_OBJECTS(30, M);
        FILL_OBJECTS(31, M);
        FILL_OBJECTS(32, M);
        FILL_OBJECTS(33, M);
        FILL_OBJECTS(34, M);
        FILL_OBJECTS(35, M);
        FILL_OBJECTS(36, M);
        FILL_OBJECTS(37, M);
        FILL_OBJECTS(38, M);
        FILL_OBJECTS(39, M);
        FILL_OBJECTS(40, M);
        FILL_OBJECTS(41, M);
        FILL_OBJECTS(42, M);
        FILL_OBJECTS(43, M);
        FILL_OBJECTS(44, M);
        FILL_OBJECTS(45, M);
        FILL_OBJECTS(46, M);
        FILL_OBJECTS(47, M);
        FILL_OBJECTS(48, M);
        FILL_OBJECTS(49, M);
        FILL_OBJECTS(50, M);
        FILL_OBJECTS(51, M);
        FILL_OBJECTS(52, M);
        FILL_OBJECTS(53, M);
        FILL_OBJECTS(54, M);
        FILL_OBJECTS(55, M);
        FILL_OBJECTS(56, M);
        FILL_OBJECTS(57, M);
        FILL_OBJECTS(58, M);
        FILL_OBJECTS(59, M);
        FILL_OBJECTS(60, M);
        FILL_OBJECTS(61, M);
        FILL_OBJECTS(62, M);
        FILL_OBJECTS(63, M);
        FILL_OBJECTS(64, M);
        FILL_OBJECTS(65, M);
        FILL_OBJECTS(66, M);
        FILL_OBJECTS(67, M);
        FILL_OBJECTS(68, M);
        FILL_OBJECTS(69, M);
        FILL_OBJECTS(70, M);
        FILL_OBJECTS(71, M);
        FILL_OBJECTS(72, M);
        FILL_OBJECTS(73, M);
        FILL_OBJECTS(74, M);
        FILL_OBJECTS(75, M);
        FILL_OBJECTS(76, M);
        FILL_OBJECTS(77, M);
        FILL_OBJECTS(78, M);
        FILL_OBJECTS(79, M);
        FILL_OBJECTS(80, M);
        FILL_OBJECTS(81, M);
        FILL_OBJECTS(82, M);
        FILL_OBJECTS(83, M);
        FILL_OBJECTS(84, M);
        FILL_OBJECTS(85, M);
        FILL_OBJECTS(86, M);
        FILL_OBJECTS(87, M);
        FILL_OBJECTS(88, M);
        FILL_OBJECTS(89, M);
        FILL_OBJECTS(90, M);
        FILL_OBJECTS(91, M);
        FILL_OBJECTS(92, M);
        FILL_OBJECTS(93, M);
        FILL_OBJECTS(94, M);
        FILL_OBJECTS(95, M);
        FILL_OBJECTS(96, M);
        FILL_OBJECTS(97, M);
        FILL_OBJECTS(98, M);
        FILL_OBJECTS(99, M);
        w.write(e);
    }
}
