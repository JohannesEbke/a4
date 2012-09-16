#include <iostream>

#include <a4/io/Tests.pb.h>
#include <a4/message.h>
#include <a4/output.h>
#include <a4/output_stream.h>
#include <a4/input.h>
#include <a4/input_stream.h>
#include <a4/input_stream.h>

#include "striping.h"

using namespace std;
using namespace a4::io;

int main(int argc, char ** argv) {

    TestDocument doc1, doc2;
    doc1.set_docid(10);
    auto *lk1 = doc1.mutable_links();
    lk1->add_forward(20);
    lk1->add_forward(40);
    lk1->add_forward(60);
    auto *n1 = doc1.add_name();
    auto *l1 = n1->add_language();
    l1->set_code("en-us");
    l1->set_country("us");
    auto *l2 = n1->add_language();
    l2->set_code("en");
    n1->set_url("http://A");
    auto *n2 = doc1.add_name();
    n2->set_url("http://B");
    auto *n3 = doc1.add_name();
    auto *l3 = n3->add_language();
    l3->set_code("en-gb");
    l3->set_country("gb");

    doc2.set_docid(20);
    auto *lk2 = doc2.mutable_links();
    lk2->add_backward(10);
    lk2->add_backward(30);
    lk2->add_forward(80);
    auto *n4 = doc2.add_name();
    n4->set_url("http://C");

    std::cout << doc1.DebugString() << std::endl;
    std::cout << doc2.DebugString() << std::endl;

    ColumnWriter test_writer;
    dissect_record(doc1, test_writer);
    dissect_record(doc2, test_writer);
    std::cout << test_writer.DebugString() << std::endl;

}
