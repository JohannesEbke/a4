#include <iostream>

#include <a4/output.h>
#include <a4/output_stream.h>
#include <a4/input.h>
#include <a4/input_stream.h>
#include <a4/message.h>

#include "Event.pb.h"

using namespace std;
using namespace a4::io;


int main(int argc, char ** argv) {
    const int N = 1000;
    {
        A4Output a4o("test_io.a4", "Event");

        shared<OutputStream> stream = a4o.get_stream();

        Event e;
        for(int i = 0; i < N; i++) {
            e.set_event_number(i);
            stream->write(e);
        }
        EventStreamInfo m;
        m.set_total_events(N);
        stream->metadata(m);
    }
    {
        A4Input in;
        in.add_file("test_io.a4");
        while (shared<InputStream> stream = in.get_stream()) {
            int cnt = 0;
            while (shared<A4Message> msg = stream->next()) {
                if (auto * te = msg->as<Event>()) {
                    auto * me = stream->current_metadata()->as<EventStreamInfo>();
                    assert(cnt++ == te->event_number());
                    assert(me->total_events() == N);
                }
            }
            if (stream->error()) throw "AJS";
        }
    }
}

