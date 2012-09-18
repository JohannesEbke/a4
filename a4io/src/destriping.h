#include <unordered_map>
#include <sstream>
#include <vector>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

using google::protobuf::Message;
using google::protobuf::FieldDescriptor;

#include <a4/dynamic_message.h>
using a4::io::FieldContent;




struct ColumnLine {
    ColumnLine(unsigned int r, unsigned int d, std::string v) : repetition_level(r), definition_level(d), value(v) {};
    unsigned int repetition_level, definition_level;
    std::string value;
};

class FieldReader {
    public:
    FieldReader(FieldReader * p, const FieldDescriptor* fd) : _parent(p), _fd(fd), _position(0) {
        if (p == NULL) {
            _definition_level = 0;
            _level = 0;
            _max_repetition_level = 0;
        } else {
            _level = p->_level + 1;
            if (fd->is_repeated()) {
                _max_repetition_level = p->_max_repetition_level + 1;
            } else {
                _max_repetition_level = p->_max_repetition_level;
            }
            if (!fd->is_required()) {
                _definition_level = p->_definition_level + 1;
            } else {
                _definition_level = p->_definition_level;
            }
        }
    }
    public:

    FieldReader* _parent;
    const FieldDescriptor * _fd;
    int _level;
    int _definition_level;
    int _position;
    int _max_repetition_level;
    std::vector<ColumnLine> _data;

    const ColumnLine& read() {
        return _data[_position++];
    }

    bool has_data() {
        return _data.size() > _position;
    }

    unsigned int definition_level() {
        return _definition_level;
    }

    unsigned int level() { // called full definition level in the paper
        return _level;
    }

    std::string name() {
        if (_fd == NULL) {
            return "root";
        } else {
            return _fd->full_name();
        }
    }
};

#define FSM_BARRIER (FieldReader*)0x1
FieldReader * last_reader;
std::stringstream record;


FieldReader* lowest_common_ancestor(FieldReader* r1, FieldReader* r2) {
    //std::cout << "Lowest common ancestor of " << r1 << " and " << r2 << " is... " << std::endl;
    //std::cout << "Levels " << r1->level() << " and " << r2->level() << " is... " << std::endl;
    // go up to the common level
    while (r1->_level < r2->_level) r2 = r2->_parent;
    while (r2->_level < r1->_level) r1 = r1->_parent;
    while (r1 != r2) {
        r1 = r1->_parent;
        r2 = r2->_parent;
    }
    //std::cout << r1 << std::endl;
    return r1;
}

class ColumnReader {
    public:
    FieldReader * root_field_reader;
    std::vector<FieldReader*> readers;
    std::map<std::pair<FieldReader*,int>, FieldReader*> fsm_transitions;

    void make_fsm() {
        for(int i = 0; i < readers.size(); i++) {
            unsigned int max_level = readers[i]->_max_repetition_level;
            FieldReader * barrier = (i == readers.size()-1) ? FSM_BARRIER : readers[i+1]; // next field after field or final FSM state otherwise
            unsigned int barrier_level = 0;
            if (barrier != FSM_BARRIER) barrier_level = lowest_common_ancestor(readers[i], barrier)->_max_repetition_level;
            for(int j = 0; j < i; j++) {
                if (readers[j]->_max_repetition_level > barrier_level) {
                    unsigned int back_level = lowest_common_ancestor(readers[j], readers[i])->_max_repetition_level;
                    fsm_transitions[std::make_pair(readers[i], back_level)] = readers[j];
                }
            }
            for(int level = barrier_level+1; level <= max_level; level++) {
                FieldReader* &target = fsm_transitions[std::make_pair(readers[i], level)];
                if (not target) {
                    target = fsm_transitions[std::make_pair(readers[i], level-1)];
                }
            }
            for (int level = 0; level < barrier_level+1; level++) {
                fsm_transitions[std::make_pair(readers[i], level)] = FSM_BARRIER;
            }
        }
        std::cout << "FSM Transitions:" << std::endl;
        foreach(auto& kv, fsm_transitions) {
            if (kv.first.first->_fd == NULL) {
                std::cout << kv.first.first;
            } else {
                std::cout << kv.first.first->_fd->full_name();
            }
            std::cout << ", " << kv.first.second << " --> ";;
            if (kv.second == FSM_BARRIER) {
                std::cout << " -- END -- ";
            } else if (kv.second->_fd == NULL) {
                std::cout << kv.second;
            } else {
                std::cout << kv.second->_fd->full_name();
            }
            std::cout << std::endl;
        }
        std::cout << ".............." << std::endl;
    }
};
        
void end_nested_records(FieldReader* r, int level) {
    for (int i = 0; i < level; i++) record << " ";
    record << "} // end " << r->_fd->full_name() << std::endl;
}

void start_nested_records(FieldReader* r, int level) {
    for (int i = 0; i < level; i++) record << " ";
    record << r->_fd->full_name() << "{" << std::endl;
}


void MoveToLevel(int newLevel, FieldReader * nextReader) {
    FieldReader * r1 = last_reader;
    int current_level = last_reader->level();
    // go down to level of next reader
    while (current_level > nextReader->level()) {
        end_nested_records(r1, current_level);
        r1 = r1->_parent;
        current_level--;
    }
    // go down in parallel to first common ancestor
    FieldReader * r2 = nextReader;
    while(r1 != r2) {
        end_nested_records(r1, current_level);
        r1 = r1->_parent;
        r2 = r2->_parent;
        current_level--;
    }
    // go up to next_reader
    FieldReader * r;
    while (current_level < newLevel) {
        current_level++;
        r = nextReader;
        for (int i = 0; i < (newLevel - current_level); i++) r = r->_parent;
        start_nested_records(r, current_level);
    }
    last_reader = r;
}

void ReturnToLevel(int newLevel) {
    //std::cout << "returning to " << newLevel << " last reader " << last_reader <<std::endl;
    FieldReader * r = last_reader;
    int current_level = last_reader->level();
    //std::cout << "from level " << current_level << std::endl;
    while (current_level > newLevel) {
        end_nested_records(r, current_level);
        r = r->_parent;
        current_level--;
    }
    last_reader = r;
}

// Reuse msg
//template <class MessageType>
std::string AssembleRecord(ColumnReader * creaders) {
    record.clear();
    last_reader = creaders->root_field_reader;
    auto* reader = creaders->readers[0];
    std::cout << "Starting with reader " << reader->name() << std::endl;
    while (reader->has_data()) {
        std::cout << "reader " << reader->name() << " has data..." << std::endl;
        const ColumnLine &l = reader->read();
        bool value_is_not_null = (l.definition_level == reader->definition_level());
        if (value_is_not_null) {
            MoveToLevel(reader->definition_level(), reader);
            for (int i = 0; i < reader->level(); i++) record << " ";
            record << l.value << std::endl;
        } else {
            MoveToLevel(reader->level(), reader);
        }
        reader = creaders->fsm_transitions[std::make_pair(reader, l.repetition_level)];
        ReturnToLevel(reader->definition_level());
    }
    ReturnToLevel(0); //End all nested records (implicit)
    return record.str();
}


