// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_SERVERS_SERVER_STATUS_HPP_
#define CLUSTERING_ADMINISTRATION_SERVERS_SERVER_STATUS_HPP_

#include <string>
#include <vector>

#include "errors.hpp"
#include <boost/shared_ptr.hpp>

#include "clustering/administration/metadata.hpp"
#include "clustering/administration/servers/server_common.hpp"
#include "clustering/administration/servers/server_metadata.hpp"
#include "clustering/administration/servers/last_seen_tracker.hpp"
#include "rdb_protocol/artificial_table/backend.hpp"
#include "rpc/semilattice/view.hpp"

class server_config_client_t;

class server_status_artificial_table_backend_t :
    public common_server_artificial_table_backend_t
{
public:
    server_status_artificial_table_backend_t(
            boost::shared_ptr< semilattice_readwrite_view_t<
                servers_semilattice_metadata_t> > _servers_sl_view,
            server_config_client_t *_server_config_client,
            watchable_map_t<peer_id_t, cluster_directory_metadata_t> *_directory_view);
    ~server_status_artificial_table_backend_t();

    bool write_row(
            ql::datum_t primary_key,
            bool pkey_was_autogenerated,
            ql::datum_t *new_value_inout,
            signal_t *interruptor,
            std::string *error_out);

private:
    bool format_row(
            name_string_t const & name,
            server_id_t const & server_id,
            server_semilattice_metadata_t const & server,
            signal_t *interruptor,
            ql::datum_t *row_out,
            std::string *error_out);

    watchable_map_t<peer_id_t, cluster_directory_metadata_t> *directory_view;
    last_seen_tracker_t last_seen_tracker;

    watchable_map_t<peer_id_t, cluster_directory_metadata_t>::all_subs_t directory_subs;
};

#endif /* CLUSTERING_ADMINISTRATION_SERVERS_SERVER_STATUS_HPP_ */

