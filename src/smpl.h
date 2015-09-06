#ifndef __SMPL_H__
#define __SMPL_H__

#include <string>

namespace smpl {

    enum CHANNEL_STATUS{
        CHANNEL_BLOCKED,
        CHANNEL_READY,
        CHANNEL_ERROR
    };

    enum ADDRESS_STATUS{
        ADDRESS_BLOCKED,
        ADDRESS_READY,
        ADDRESS_ERROR
    };

    //This exception is thrown when attempting to construct a Remote_Address
    //or Local_Address with semantically invalid parameters.  This allows us
    //to ensure that all constructed addresses are valid.
    class Bad_Address {};

    //This exception is thrown when sending or receiving fails, either completly
    //or partially.
    class Transport_Failed {};

    //This exception is thrown when connecting to a Remote_Address fails
    class Connection_Failed {};

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
            void send(const std::string &msg){
                const size_t r = _send(msg);
                if( r != msg.size() ){
                    throw Transport_Failed();
                }
                return;
            }

            //This function receives the next complete message. Regardless of
            //the mechanics of the underlying transport layer recv() should
            //return a single complete message. If no complete message is
            //available, this function should block.
            //
            //Return Value: The next complete message from the Remote Peer. An
            //empty string indicates that an error has occured.
            std::string recv(){
                std::string incoming_msg;
                const auto r = _recv(incoming_msg);
                if( r < 0 ){
                    throw Transport_Failed();
                }
                return incoming_msg;
            }

            //This function blocks and waits until a message is ready to be
            //recv()ed, (wait() will not block, not unlike select()).
            //
            //Return Value: True if a message is waiting and recv() will not
            //block. False if there's been an error i.e. the transport layer
            //has failed.
            virtual CHANNEL_STATUS wait() noexcept = 0;

        private:

            virtual ssize_t _send(const std::string &msg) noexcept = 0;
            virtual ssize_t _recv(std::string &msg) noexcept = 0;

    };

    //A class for specifying a remote entity to connect to.
    class Remote_Address{

        public:
            //This function yields a Channel whose remote end is specified by
            //this Remote_Address. If the entity specified by Remote_Address
            //is unreachable (e.g. does not exist) this function will fail
            //immediately. This function will block until the Local_Address
            //corresponding to this Remote_Address listen()s.
            //
            //Return Value: A pointer to a valid Channel object
            Channel* connect(){
                Channel* connection = _connect();
                if(connection == nullptr){
                    throw Connection_Failed();
                }
                else{
                    return connection;
                }
            }

        private:
            virtual Channel* _connect() noexcept = 0;

    };

    //A class for specifying a local "address" to recieve incoming connections
    //on.
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
            //attempting to connect to this Local_Address.
            Channel* listen(){
                Channel* connection = _listen();
                if(connection == nullptr){
                    throw Connection_Failed();
                }
                else{
                    return connection;
                }
            }

            //This function performs a non-blocking check to see if there are
            //any available Remote Peers blocked attempting a connection.
            //
            //Return Value: True if there is an incoming connection and
            //listen() would not block. False if there is no incoming connection
            //and listen() would block.
            virtual ADDRESS_STATUS check() noexcept = 0;

        private:
            virtual Channel* _listen() noexcept = 0;

    };

}

#endif
