/* Copyright 2016 Eunsoo Park (esevan.park@gmail.com). All rights reserved
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

#ifndef INC_NETWORK_ADAPTER_H_
#define INC_NETWORK_ADAPTER_H_

#include <thread>
#include <mutex>

namespace cm {
typedef enum {
  kDevOff = 0,
  kDevOn = 1,
  kDevDiscon = 2,
  kDevDisconnecting = 3,
  kDevConnecting = 4,
  kDevCon = 5,
} DevState;

typedef enum {
  kLOW = 0,
  kMID = 1,
  kHIG = 2
} NaVal;

typedef void (*DevStatCb)(DevState res);

class NetworkAdapter {
  friend class NetworkManager;
 public:
  NetworkAdapter();
  ~NetworkAdapter();

  DevState get_stat(void);
  virtual void set_data_adapter(void) final;
  virtual void set_control_adapter(void) final;

 protected:
  /**
   * Followings should be implemented on a child class
   * return value is either true (on success) or false (on failure)
   * Don't worry about blocking operation.
   * A single method is called at a single thread.
   * If your adapter failed sending a packet, just return false.
   * Other adapters installed on the system is going to be in charge of it.
   */
  char dev_name[256];
  uint16_t dev_id;
  virtual bool device_on(void) = 0;
  virtual bool device_off(void) = 0;

  virtual bool make_connection(void) = 0;
  virtual bool close_connection(void) = 0;

  // If connection is closed, send and recv both should be failed
  virtual int send(const void *buf, size_t len) = 0;
  virtual int recv(void *buf, size_t len) = 0;

  // Property information
  virtual uint16_t get_id(void) = 0;
  virtual void set_controllable(void) final;
  virtual void on_control_recv(const void *buf, size_t len) = 0;
  virtual void send_ctrl_msg(const void *buf, int len) final;

 private:
  typedef enum {
    kATUninitialized = 0,
    kATInitialized = 1,
    kATCtrlable = 2,
    kATCtrl = 4,
  } AdapterType;
  std::thread *th_sender;
  std::thread *th_recver;

  int at;
  DevState stat;
  std::mutex dev_on_lock;
  std::mutex dev_off_lock;
  std::mutex connect_lock;
  std::mutex close_lock;

  DevStatCb device_on_cb;
  DevStatCb device_off_cb;
  DevStatCb make_connection_cb;
  DevStatCb close_connection_cb;

  
  void run_sender(void);
  void run_recver(void);


  virtual void dev_switch(DevState stat, DevStatCb cb) final;
  void dev_on(void);
  void dev_off(void);

  void _connect(void);
  void _close(void);

  void return_sending_failed_packet(void *segment);
};
}  /* namespace cm */

#endif  /* INC_NETWORK_ADAPTER_H_ */
