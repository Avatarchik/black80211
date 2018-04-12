/* add your code here */

#include "Black80211Control.hpp"

OSDefineMetaClassAndStructors(Black80211Control, IO80211Controller);
#define super IO80211Controller

bool Black80211Control::init(OSDictionary *parameters) {
    return super::init(parameters);
}

void Black80211Control::free() {
    
    super::free();
}

bool Black80211Control::start(IOService *provider) {
    IOLog("ARRRRRRRRRRRRRHHHHHHHHHHHHHHHHHHHH");
    
    if (!super::start(provider))
        return false;
    
    fWorkloop = getWorkLoop();

    if (fWorkloop == 0) {
        IOLog("No workloop!!\n");
        return false;
    }

    fCommandGate = IOCommandGate::commandGate(this, (IOCommandGate::Action)tsleepHandler);
    if (fCommandGate == 0) {
        IOLog("No command gate!!\n");
        return false;
    }
    fWorkloop->addEventSource(fCommandGate);

    
    attachInterface((IONetworkInterface**) &fInterface, /* attach to DLIL = */ true);
//    registerService();

    
    return true;
}

void Black80211Control::stop(IOService *provider) {
    if (fCommandGate) {
        if (fWorkloop) {
            fWorkloop->removeEventSource(fCommandGate);
        }
        
        fCommandGate->release();
        fCommandGate = NULL;
    }
    
    if (fWorkloop) {
        fWorkloop->release();
        fWorkloop = NULL;
    }
    
    if (fOutputQueue) {
        fOutputQueue->release();
        fOutputQueue = NULL;
    }
    
    detachInterface((IONetworkInterface *)fInterface);
    
    super::stop(provider);
}

IOReturn Black80211Control::enable(IONetworkInterface *interface) {
    return kIOReturnSuccess;
}

IOReturn Black80211Control::disable(IONetworkInterface *interface) {
    return kIOReturnSuccess;
}

IOReturn Black80211Control::getHardwareAddress(IOEthernetAddress *addr) {
    return kIOReturnSuccess;
}

IOReturn Black80211Control::getHardwareAddressForInterface(IO80211Interface* netif, IOEthernetAddress* addr) {
    return getHardwareAddress(addr);
}

SInt32 Black80211Control::apple80211RequestIoctl( UInt32 request_type, int request_number, IO80211Interface* interface, void* data ) {
    return kIOReturnSuccess;
}

IOReturn Black80211Control::apple80211Request_SET(int request_number, void* data) {
    return kIOReturnSuccess;
}

IOReturn Black80211Control::apple80211Request_GET(int request_number, void* data) {
    return kIOReturnSuccess;
}

IO80211WorkLoop* Black80211Control::getWorkLoop() {
    if (!fWorkloop) {
        fWorkloop = IO80211WorkLoop::workLoop();
    }
    
    return fWorkloop;
}

IO80211Interface* Black80211Control::getInterface() {
    return fInterface;
}

UInt32 Black80211Control::outputPacket(mbuf_t m, void *param) {
    freePacket(m);
    return kIOReturnSuccess;
}

IOOutputQueue* Black80211Control::createOutputQueue() {
    
    if (fOutputQueue == 0) {
        fOutputQueue = IOGatedOutputQueue::withTarget(this, getWorkLoop());
    }
    return fOutputQueue;

}


IOReturn Black80211Control::getMaxPacketSize( UInt32 *maxSize ) const { *maxSize = 1500; return kIOReturnSuccess; }
IOReturn Black80211Control::setPromiscuousMode(IOEnetPromiscuousMode mode) { return kIOReturnSuccess; }
IOReturn Black80211Control::setMulticastMode(IOEnetMulticastMode mode) { return kIOReturnSuccess; }
IOReturn Black80211Control::setMulticastList(IOEthernetAddress* addr, UInt32 len) { return kIOReturnSuccess; }
SInt32   Black80211Control::monitorModeSetEnabled(IO80211Interface* interface, bool enabled, UInt32 dlt) { return kIOReturnSuccess; }

const OSString*    Black80211Control::newVendorString    ( ) const    { return OSString::withCString("Voodoo(R)"); }
const OSString*    Black80211Control::newModelString        ( ) const    { return OSString::withCString("Wireless Device(TM)"); }
const OSString*    Black80211Control::newRevisionString    ( ) const    { return OSString::withCString("1.0"); }

IOReturn Black80211Control::registerWithPolicyMaker
( IOService* policyMaker )
{
    static IOPMPowerState powerStateArray[ 2 ] = {
        { 1,0,0,0,0,0,0,0,0,0,0,0 },
        { 1,kIOPMDeviceUsable,kIOPMPowerOn,kIOPMPowerOn,0,0,0,0,0,0,0,0 }
    };
    return policyMaker->registerPowerDriver( this, powerStateArray, 2 );
}


IOReturn Black80211Control::tsleepHandler(OSObject* owner, void* arg0 = 0, void* arg1 = 0, void* arg2 = 0, void* arg3 = 0) {
    
    return kIOReturnSuccess;

}


