/* -*- c++ -*- */
/*
 * Copyright 2014 Communications Engineering Lab, KIT.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "msg_gate_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm> // needed for std::find

namespace gr {
namespace radar {

msg_gate::sptr msg_gate::make(std::vector<std::string> keys,
                              std::vector<float> val_min,
                              std::vector<float> val_max)
{
    return gnuradio::make_block_sptr<msg_gate_impl>(keys, val_min, val_max);
}

/*
 * The private constructor
 */
msg_gate_impl::msg_gate_impl(std::vector<std::string> keys,
                             std::vector<float> val_min,
                             std::vector<float> val_max)
    : gr::block(
          "msg_gate", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0))
{
    d_keys = keys;
    d_val_min = val_min;
    d_val_max = val_max;

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in, boost::bind(&msg_gate_impl::handle_msg, this, _1));

    // Register output message port
    d_port_id_out = pmt::mp("Msg out");
    message_port_register_out(d_port_id_out);
}

void msg_gate_impl::handle_msg(pmt::pmt_t msg)
{
    // FIXME: remake me, pretty bad code
    size_t msg_size = pmt::length(msg);
    std::vector<pmt::pmt_t> msg_parts_f32_key;
    std::vector<std::vector<float>> msg_parts_f32_val; // take f32 vectors of msg
    std::vector<pmt::pmt_t> msg_parts_rest;            // take rest
    std::vector<int> index_remove_items;

    // Push back float32 vectors in msg to separate vectors (only valid type for data) //
    // FIXME: allow other data types?
    for (int k = 0; k < msg_size; k++) {
        if (pmt::is_f32vector(pmt::nth(1, pmt::nth(k, msg)))) {
            msg_parts_f32_key.push_back(
                pmt::nth(0, pmt::nth(k, msg))); // save key (pmt symbol)
            msg_parts_f32_val.push_back(pmt::f32vector_elements(
                pmt::nth(1, pmt::nth(k, msg)))); // save value of f32 vec
        } else {
            msg_parts_rest.push_back(pmt::nth(k, msg));
        }
    }

    if (msg_parts_f32_key.size() != 0) {

        // Filter f32 vectors with string keys and save index of items out of boundries
        size_t key_size;
        key_size = d_keys.size();
        size_t counter, size_vec;
        for (int k = 0; k < msg_parts_f32_key.size(); k++) { // go through msg_parts keys
            for (int p = 0; p < key_size; p++) {             // go through string keys
                if (d_keys[p] ==
                    pmt::symbol_to_string(
                        msg_parts_f32_key[k])) { // if matching key is found apply filter
                    for (int q = 0; q < msg_parts_f32_val[k].size();
                         q++) { // look for items out of boundries and store index
                        if (msg_parts_f32_val[k][q] < d_val_min[p] ||
                            msg_parts_f32_val[k][q] > d_val_max[p]) {
                            if (index_remove_items.end() ==
                                std::find(index_remove_items.begin(),
                                          index_remove_items.end(),
                                          q)) { // check if item is not already in vector
                                index_remove_items.push_back(q);
                            }
                        }
                    }
                }
            }
        }

        // Copy valid items from f32 vectors
        std::vector<std::vector<float>> f32_hold;
        f32_hold.resize(msg_parts_f32_val.size());

        for (int p = 0; p < msg_parts_f32_val[0].size(); p++) {
            if (index_remove_items.end() ==
                std::find(index_remove_items.begin(),
                          index_remove_items.end(),
                          p)) { // if index is not in vector for removed items
                for (int k = 0; k < msg_parts_f32_val.size(); k++) {
                    f32_hold[k].push_back(msg_parts_f32_val[k][p]);
                }
            }
        }

        if (f32_hold[0].size() != 0) { // if there is any item left after filtering
            // Repack msg
            pmt::pmt_t msg_out_rest, msg_out_f32, msg_out;
            std::vector<pmt::pmt_t> f32_hold_pmt;
            bool is_msg_rest;
            is_msg_rest = true;

            // Repack rest pmts (not f32)
            if (msg_parts_rest.size() != 0) {
                msg_out_rest = pmt::list1(msg_parts_rest[0]);
                for (int k = 1; k < msg_parts_rest.size(); k++) {
                    msg_out_rest = pmt::list_add(msg_out_rest, msg_parts_rest[k]);
                }
            }

            // Repack f32 pmts
            f32_hold_pmt.resize(f32_hold.size());
            for (int k = 0; k < f32_hold.size(); k++) {
                f32_hold_pmt[k] =
                    list2(msg_parts_f32_key[k],
                          pmt::init_f32vector(f32_hold[k].size(), f32_hold[k]));
            }
            if (is_msg_rest) {
                msg_out = msg_out_rest;
                for (int k = 0; k < f32_hold_pmt.size(); k++) {
                    msg_out = list_add(msg_out, f32_hold_pmt[k]);
                }
            } else {
                msg_out = list1(f32_hold_pmt[0]);
                for (int k = 1; k < f32_hold_pmt.size(); k++) {
                    msg_out = pmt::list_add(msg_out, f32_hold_pmt[k]);
                }
            }

            message_port_pub(d_port_id_out, msg_out);
        }

    } // end if msg_parts_f32.size() != 0
}

/*
void
msg_gate_impl::handle_msg(pmt::pmt_t msg){
        d_msg_size = pmt::length(msg);
        d_msg_part_hold.clear();
        d_msg_part_hold.resize(0);

        for(int k=0; k<d_msg_size; k++){ // go through msg
                d_msg_part = pmt::nth(k,msg);
                d_msg_key = pmt::symbol_to_string(pmt::nth(0,d_msg_part)); // get key
                d_key_found = false;
                for(int l=0; l<d_keys.size(); l++){ // search for key in input keys
                        if(d_msg_key==d_keys[l]){
                                // If key is found check for vector and repack msg
                                d_key_found = true;
                                if(pmt::is_f32vector(pmt::nth(1,d_msg_part))){ // only f32
vector works d_vec_size = pmt::length(pmt::nth(1,d_msg_part)); d_val_store.clear();
                                        d_val_store.resize(d_vec_size);
                                        d_val_store =
pmt::f32vector_elements(pmt::nth(1,d_msg_part));
                                        // Check if element is in boundries
                                        d_val_accept.clear();
                                        d_val_accept.resize(0);
                                        for(int m=0; m<d_vec_size; m++){
                                                if((d_val_store[m]>=d_val_min[l])&&(d_val_store[m]<=d_val_max[l])){
                                                        d_val_accept.push_back(d_val_store[m]);
                                                }
                                        }
                                }
                                else{
                                        1; // FIXME: throw warning for wrong datatype
                                }
                        } // end repacking
                } // end checking keys

                // Store pmt
                if(d_key_found){
                        d_msg_part_hold.push_back(pmt::list2(pmt::nth(0,d_msg_part),pmt::init_f32vector(d_val_accept.size(),d_val_accept)));
// push back new pmt if key is found
                }
                else{
                        d_msg_part_hold.push_back(d_msg_part); // push back old msg part
if not
                }

        } // end check msg

        // Rebuild msg and push to output
        pmt::pmt_t msg_out;
        if(d_msg_part_hold.size()==1){
                msg_out = list1(d_msg_part_hold[0]);
        }
        else if(d_msg_part_hold.size()==2){
                msg_out = pmt::list2(d_msg_part_hold[0],d_msg_part_hold[1]);
        }
        else if(d_msg_part_hold.size()==3){
                msg_out =
pmt::list3(d_msg_part_hold[0],d_msg_part_hold[1],d_msg_part_hold[2]);
        }
        message_port_pub(d_port_id_out,msg_out);
}
*/

/*
 * Our virtual destructor.
 */
msg_gate_impl::~msg_gate_impl() {}

} /* namespace radar */
} /* namespace gr */
