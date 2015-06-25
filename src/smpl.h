#ifndef __SMPL_H__
#define __SMPL_H__

#include <string>

namespace smpl {

    //This exception is thrown when attempting to construct a Remote_Address
    //or Local_Address with semantically invalid parameters.  This allows us
    //to ensure that all constructed addresses are valid.
    class Bad_Address {};

    //A bi-directional pipe for 'atomic' message passing between this Channel
    //and its Remote Peer.  The remote end is set at construction time and
    //fixed.
    class Channel{

        public:

            Channel() {};
            virtual ~Channel() noexcept {};

            //Copy constructor deleted since copying a Channel would likely
            //break synchronous access to the underlying transport
            Channel(const Channel&) = delete;

            //This function sends a complete message to be recv()ed by the
            //Remote Peer. Regardless of the mechanics of the underlying
            //transport this function should be non-blocking.
            //
            //msg: A non-empty string to be sent to the Remote Peer
            //Return Value: Length of message sent, should match msg.size().
            //A return value not matching msg.size() indicates an error.
            ssize_t send(const std::string &msg) noexcept {
                if(msg.empty()){
                    return -1;
                }
                else
                    return _send(msg);
            }

            //This function receives the next complete message. Regardless of
            //the mechanics of the underlying transport layer recv() should
            //return a single complete message. If no complete message is
            //available, this function should block.
            //
            //Return Value: The next complete message from the Remote Peer. An
            //empty string indicates that an error has occured.
            virtual std::string recv() noexcept = 0;

            //This function blocks and waits until a message is ready to be
            //recv()ed, (recv() will not block, not unlike select()).
            //
            //Return Value: True if a message is waiting and recv() will not
            //block. False if there's been an error i.e. the transport layer
            //has failed.
            virtual bool wait() noexcept = 0;

        private:

            virtual ssize_t _send(const std::string &msg) noexcept = 0;

    };

    class Remote_Address{

        public:
            //This function yields a Channel whose remote end is specified by
            //this Remote_Address. This function should not block.
            //
            //BUG: If this should not block, then Local_Address::check() is
            //bullshit
            //
            //Return Value: A pointer to a valid Channel object, or nullptr
            //indicating error.
            virtual Channel* connect() noexcept = 0;

    };

    class Local_Address{

        public:

            Local_Address() {};
            virtual ~Local_Address() noexcept {};

            //Copy constructor deleted because generally an instance of
            //Local_Address should "own" that local address.
            Local_Address(const Local_Address&) = delete;

            //This function blocks and waits for an incoming connection. Upon
            //a connection attempt it establishes a Channel with the remote end
            //and returns a pointer to it.
            //
            //Return Value: A pointer to a valid Channel to a Remote Peer
            //attempting to connect to this Local_Address. nullptr indicates
            //an error.
            virtual Channel* listen() noexcept = 0;

            //This function performs a non-blocking check to see if there are
            //any available Remote Peers blocked attempting a connection.
            //
            //BUG: If Remote Peers dno't block connect()ing, then
            //Local_Address::check() is always false...
            //
            //Return Value: True if there is an incoming connection and
            //listen() would not block. False if there is no incoming connection
            //and listen() would block.
            virtual bool check() noexcept = 0;

    };

}

#endif
