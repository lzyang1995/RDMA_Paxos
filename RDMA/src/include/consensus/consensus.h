#ifndef CONSENSUS_H

#define CONSENSUS_H
#include "../util/common-header.h"

typedef uint64_t db_key_type;
struct consensus_component_t;

typedef enum con_role_t{
    LEADER = 0,
    SECONDARY = 1,
}con_role;

struct consensus_component_t* init_consensus_comp(const char* config_path, const char* log_path, int64_t node_id, const char* start_mode);
int rsm_op(struct consensus_component_t* comp, void* data, size_t data_size);
void handle_accept_req(struct consensus_component_t* comp);

#endif