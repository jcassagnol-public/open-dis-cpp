/// Copyright goes here
/// License goes here
/// @author John K. Grant

#ifndef _dcl_dis_incoming_message_
#define _dcl_dis_incoming_message_

#include <utils/IBufferProcessor.h>   // for base class
#include <utils/IPduBank.h> 
#include <map>                      // for member
#include <utils/Endian.h>             // for internal type
#include <dis6/msLibMacro.h>         // for library symbols
#include <utils/PDUType.h>

#include <utils/Dis6Bank.h>

#include <boost/asio.hpp>

namespace DIS
{
   class Pdu;
   class IPacketProcessor;
   class DataStream;

   /// A framework for routing the packet to the correct processor.
   class EXPORT_MACRO IncomingMessage : public IBufferProcessor
   {
   public:
      /// the container type for supporting processors.
      typedef std::multimap<unsigned char,IPacketProcessor*> PacketProcessorContainer;
      
      /// the container type for supporting PDU banks.
      typedef std::multimap<unsigned char,IPduBank*> PduBankContainer;

      IncomingMessage();
      ~IncomingMessage();

      void Process(const char* buf, unsigned int size, Endian e);

      /// registers the ipp instance to process packets with the id
      /// @return 'true' if the pair of parameters were not found in the container and were addded.  'false' if the pair was found.
      bool AddProcessor(unsigned char id, IPacketProcessor* pp);

      /// unregisters the ipp instance for packets with the id.  it does not clean up the delete the ipp.
      /// @return 'true' if the pair of parameters were found in the container and removed.  'false' if the pair was not found.
      bool RemoveProcessor(unsigned char id, const IPacketProcessor* pp);

      /// registers the PDU bank instance to provide the PDU object
      /// @return 'true' if the pair of parameters were not found in the container and were addded.  'false' if the pair was found.
      bool AddPduBank(unsigned char pdu_type, IPduBank* pduBank);

      /// unregisters the PDU bank instance
      /// @return 'true' if the pair of parameters were found in the container and removed.  'false' if the pair was not found.
      bool RemovePduBank(unsigned char pdu_type, const IPduBank* pduBank);

      PacketProcessorContainer& GetProcessors();
      const PacketProcessorContainer& GetProcessors() const;

      PduBankContainer& GetPduBanks();
      const PduBankContainer& GetPduBanks() const;

      /**
       * @brief Does the same thing as Process but with a static callback to an object that takes a Pdu* instead of the dynamic processor dispatch.
       * 
       * @tparam callback the function to process the resulting PDU with.
       * @tparam logger the function to use to log with. Will send a nlohmann json message. Will be called on deserialize errors.
       * @param buf the buffer from the network.
       * @param size the size of the buffer.
       * @param e the endianness of the platform.
       */
      template<typename callback, typename logger>
      void StaticCallbackProcess(const boost::asio::const_buffer &buf, unsigned int size, Endian e, callback function, logger logFunc)
      {
         if (size < PDU_TYPE_POSITION)
         {
            return;
         }

         try
         {
            DataStream ds(static_cast<const char*>(buf.data()), size, e);

            while (ds.GetReadPos() < ds.size() - PDU_TYPE_POSITION)
            {
               unsigned char pdu_type = ds[PDU_TYPE_POSITION];
               Pdu* constructor = Dis6Bank::GetStaticPDU(static_cast<DIS::PDUType>(pdu_type));
               if (!constructor)
               {
                  return;
               }
               constructor->unmarshal(ds);
               function(constructor);
               auto current_stream_pos = ds.GetReadPos();
               // increment the pointer for padding -> DIS PDUs are padded on an 8 byte boundrey
               auto padding_required = current_stream_pos % 8;

               // size check to prevent overflow reads
               if ((ds.size() <= current_stream_pos + padding_required) || (current_stream_pos + padding_required < current_stream_pos))
               {
                  return;
               }
               
               while (padding_required)
               {
                  char unused_padding;
                  ds >> unused_padding;
                  padding_required--;
               }
            }
         }
         catch (const std::exception& e)
         {
            nlohmann::json log_message = {
               {"exception", e.what()},
               {"buffer_len", size}
            };
            logFunc(log_message);
         }
      }

   private:
      // the DIS specification says the type is known for all PDUs at the 3rd byte of the PDU buffer.
      static constexpr unsigned int PDU_TYPE_POSITION = 2;
      typedef std::pair<PacketProcessorContainer::iterator, PacketProcessorContainer::iterator> PacketProcessIteratorPair;
      PacketProcessorContainer _processors;
      
      typedef std::pair<PduBankContainer::iterator, PduBankContainer::iterator> PduBankIteratorPair;
      PduBankContainer _pduBanks;

      void SwitchOnType(DIS::PDUType pdu_type, DataStream& ds);

      /// Searches the proccesor container multimap for a matching container and returns the iterator
      bool FindProccessorContainer(unsigned char id, const IPacketProcessor* pp, PacketProcessorContainer::iterator &containerIter);

      /// Searches the PDU bank container multimap for a matching container and returns the iterator
      bool FindPduBankContainer(unsigned char pdu_type, const IPduBank* pduBank, PduBankContainer::iterator &containerIter);
   };

}

#endif  // _dcl_dis_incoming_message_

