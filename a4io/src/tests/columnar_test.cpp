#include <iostream>

#include <a4/io/Tests.pb.h>
#include <a4/message.h>
#include <a4/output.h>
#include <a4/output_stream.h>
#include <a4/input.h>
#include <a4/input_stream.h>
#include <a4/input_stream.h>

#include "striping.h"
#include "destriping.h"

using namespace std;
using namespace a4::io;

void writer_to_reader(std::vector<FieldReader*> &readers, const ColumnWriter& wr, FieldReader* parent = NULL) {
    auto r = new FieldReader(parent, wr._fd);
    readers.push_back(r);
    foreach (auto& w, wr._writers) {
        writer_to_reader(readers, *w.second, r);
    }
}


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
    //std::cout << test_writer.DebugString() << std::endl;
    test_writer.Dump();

    ColumnReader test_reader;
    writer_to_reader(test_reader.readers, test_writer);
    test_reader.root_field_reader = test_reader.readers[0];
    test_reader.make_fsm();


    foreach(auto &rd, test_reader.readers) {
        if (rd->_fd and rd->_fd->full_name() == "a4.io.TestName.Url") {
            std::cout << "written " << rd->_fd->full_name() << std::endl;
            rd->_data.push_back(ColumnLine(0,2,"http://A"));
            rd->_data.push_back(ColumnLine(0, 2, "http://A"));
            rd->_data.push_back(ColumnLine(1, 2, "http://B"));
            rd->_data.push_back(ColumnLine(1, 1 , "" ));
            rd->_data.push_back(ColumnLine(0, 2, "http://C"));
        }
        if (rd->_fd and rd->_fd->full_name() == "a4.io.TestLanguage.Country") {
            std::cout << "written " << rd->_fd->full_name() << std::endl;
            rd->_data.push_back(ColumnLine(0, 3, "us"));
            rd->_data.push_back(ColumnLine(2, 2 , "" ));
            rd->_data.push_back(ColumnLine(1, 1 , "" ));
            rd->_data.push_back(ColumnLine(1, 3, "gb"));
        }
        if (rd->_fd and rd->_fd->full_name() == "a4.io.TestLanguage.Code") {
            std::cout << "written " << rd->_fd->full_name() << std::endl;
            rd->_data.push_back(ColumnLine(0, 2, "en-us"));
            rd->_data.push_back(ColumnLine(2, 2, "en"));
            rd->_data.push_back(ColumnLine(1, 1 , "" ));
            rd->_data.push_back(ColumnLine(1, 2, "en-gb"));
        }
        if (rd->_fd and rd->_fd->full_name() == "a4.io.TestLinks.Backward") {
            std::cout << "written " << rd->_fd->full_name() << std::endl;
            rd->_data.push_back(ColumnLine(0, 1 , "" ));
            rd->_data.push_back(ColumnLine(0, 2, "10"));
            rd->_data.push_back(ColumnLine(1, 2, "30"));
        }
        if (rd->_fd and rd->_fd->full_name() == "a4.io.TestLinks.Forward") {
            std::cout << "written " << rd->_fd->full_name() << std::endl;
            rd->_data.push_back(ColumnLine(0, 2, "20"));
            rd->_data.push_back(ColumnLine(1, 2, "40"));
            rd->_data.push_back(ColumnLine(1, 2, "60"));
            rd->_data.push_back(ColumnLine(0, 2, "80"));
        }
        if (rd->_fd and rd->_fd->full_name() == "a4.io.TestDocument.DocId") {
            std::cout << "written " << rd->_fd->full_name() << std::endl;
            rd->_data.push_back(ColumnLine(0, 0, "10"));
            rd->_data.push_back(ColumnLine(0, 0, "20"));
        }
    }

    std::cout << AssembleRecord(&test_reader) << std::endl;
}
