/* Copyright 2015-2016 Eunsoo Park (esevan.park@gmail.com). All rights reserved
 * 
 * Contact: Eunsoo Park (esevan.park@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DEVICE_INC_RFCOMM_OVER_BT_H_
#define DEVICE_INC_RFCOMM_OVER_BT_H_

#include <network_adapter.h>

#include <dbug_log.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

namespace cm {
class RfcommServerOverBt : public NetworkAdapter {
 public:
  RfcommServerOverBt (uint16_t id, char *svc_uuid);

 private:
  int port;
  sdp_session_t *session;
  uuid_t svc_uuid;

  int serv_sock, cli_sock;

  char dev_name[256];

  bool device_on(void);
  bool device_off(void);
  bool make_connection(void);
  bool close_connection(void);
  int send(const void *buf, size_t len);
  int recv(void *buf, size_t len);
  uint16_t get_id(void);
  void on_control_recv(const void *buf, size_t len);

  int str2uuid(char *str, uuid_t *uuid);
  int uuid2strn(uuid_t *uuid, char *str, int len);

  int bt_register_service(void);
  int bt_dynamic_bind_rc(void);
  int bt_open(void);
};
}  /* namespace cm */

#endif  //DEVICE_INC_RFCOMM_OVER_BT_H_
