#ifndef CLICK_LAUNCHRTRSENDER_HH
#define CLICK_LAUNCHRTRSENDER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <elements/local/launch.hh>
CLICK_DECLS

/*
 * =c
 *
 * launchrequester()
 *
 */

class LaunchRTRSender : public Element { public:

  LaunchRTRSender();
  ~LaunchRTRSender();

  const char *class_name() const		{ return "LaunchRTRSender"; }
  const char *port_count() const		{ return "0/1"; }
  const char *processing() const		{ return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  bool can_live_reconfigure() const		{ return true; }
  int initialize(ErrorHandler *);

//Function called by LaunchRouter to send request to all neighbors(broadcast)
  void send_RTR();
  

 private:

  launch_ctrl_hdr _lh;


};

CLICK_ENDDECLS
#endif
