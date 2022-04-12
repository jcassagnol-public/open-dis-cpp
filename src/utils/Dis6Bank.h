#ifndef _dcl_dis_bank_h_
#define _dcl_dis_bank_h_

#include <dis6/Pdu.h>

#include <dis6/AcknowledgePdu.h>
#include <dis6/AcknowledgeReliablePdu.h>
#include <dis6/ActionRequestPdu.h>
#include <dis6/ActionRequestReliablePdu.h>
#include <dis6/ActionResponsePdu.h>
#include <dis6/ActionResponseReliablePdu.h>
#include <dis6/AggregateStatePdu.h>
#include <dis6/ArealObjectStatePdu.h>
#include <dis6/CollisionElasticPdu.h>
#include <dis6/CollisionPdu.h>
#include <dis6/CommentPdu.h>
#include <dis6/CommentReliablePdu.h>
#include <dis6/CreateEntityPdu.h>
#include <dis6/CreateEntityReliablePdu.h>
#include <dis6/DataPdu.h>
#include <dis6/DataQueryPdu.h>
#include <dis6/DataQueryReliablePdu.h>
#include <dis6/DataReliablePdu.h>
#include <dis6/DesignatorPdu.h>
#include <dis6/DetonationPdu.h>
#include <dis6/ElectromagneticEmissionsPdu.h>
#include <dis6/EntityStatePdu.h>
#include <dis6/EntityStateUpdatePdu.h>
#include <dis6/EnvironmentalProcessPdu.h>
#include <dis6/EventReportPdu.h>
#include <dis6/EventReportReliablePdu.h>
#include <dis6/FastEntityStatePdu.h>
#include <dis6/FirePdu.h>
#include <dis6/GriddedDataPdu.h>
#include <dis6/IffAtcNavAidsLayer1Pdu.h>
#include <dis6/IffAtcNavAidsLayer2Pdu.h>
#include <dis6/IntercomControlPdu.h>
#include <dis6/IntercomSignalPdu.h>
#include <dis6/IsGroupOfPdu.h>
#include <dis6/IsPartOfPdu.h>
#include <dis6/LinearObjectStatePdu.h>
#include <dis6/LogisticsPdu.h>
#include <dis6/MinefieldDataPdu.h>
#include <dis6/MinefieldQueryPdu.h>
#include <dis6/MinefieldResponseNackPdu.h>
#include <dis6/MinefieldStatePdu.h>
#include <dis6/Pdu.h>
#include <dis6/PointObjectStatePdu.h>
#include <dis6/ReceiverPdu.h>
#include <dis6/RecordQueryReliablePdu.h>
#include <dis6/RemoveEntityPdu.h>
#include <dis6/RemoveEntityReliablePdu.h>
#include <dis6/RepairCompletePdu.h>
#include <dis6/RepairResponsePdu.h>
#include <dis6/ResupplyCancelPdu.h>
#include <dis6/ResupplyOfferPdu.h>
#include <dis6/ResupplyReceivedPdu.h>
#include <dis6/SeesPdu.h>
#include <dis6/ServiceRequestPdu.h>
#include <dis6/SetDataPdu.h>
#include <dis6/SetDataReliablePdu.h>
#include <dis6/SetRecordReliablePdu.h>
#include <dis6/SignalPdu.h>
#include <dis6/StartResumePdu.h>
#include <dis6/StartResumeReliablePdu.h>
#include <dis6/StopFreezePdu.h>
#include <dis6/StopFreezeReliablePdu.h>
#include <dis6/TransferControlRequestPdu.h>
#include <dis6/TransmitterPdu.h>
#include <dis6/UaPdu.h>

#include <utils/PDUType.h>

namespace DIS
{
    /// houses instances for the set of known PDU classes to be returned
    /// when provided with the PDU type's identifier value.
    class EXPORT_MACRO Dis6Bank
    {
    public:
        /// finds the PDU instance corresponding to the identifier
        /// @param pdu_type the 8-bit PDU type identifier
        /// @return NULL when the pdu_type is unknown.
        ///\todo make this parameter just 'unsigned char' since that will be easier to generate.
        static Pdu* GetStaticPDU( DIS::PDUType pdu_type );

        /// @brief parses out JSON into PDUs. Throws if invalid.
        /// @param in_json the JSON to parse into a PDU
        /// @return the PDU casted back to a base PDU pointer, throws on invalid input.
        static std::unique_ptr<Pdu> GetPDUFromJSON(const nlohmann::json& in_json);

        /// @brief parses out PDU into JSON. Throws if PDU is in invalid state.
        /// @param in_pdu the PDU to parse into a JSON
        /// @return the JSON representation of the PDU, throws on invalid input.
        static nlohmann::json GetJSONFromPDU(Pdu& in);

        /// @brief This is template magic. Call this method on a given PDU reference, and will call
        /// the auto template function with the appropriate sub-class dynamic type.
        /// Useful for cases where a parse function does not implement a virtual method but needs to be called with an argument-dependent lookup.
        /// 
        /// @tparam funcOp the auto lambda function to parse based on type. If using argument-dependent lookup, must implement _all_ of the respective overloads.
        /// While you can technically call this with a function that takes a reference to the PDU class... maswell just call it directly at that point instead of this nonsense :D.
        /// @param in the PDU to transform in some way.
        /// @param function the function to run on the PDU, dependent on the type of the PDU given. Dynamically dispatches the correct overload of the function.
        /// Basically the dumbest way to do virtual functions.
        template<typename funcOp>
        static void parseObjectGivenType(Pdu& in, funcOp function) {
            switch(in.getPduType())
            {
                case PDU_ENTITY_STATE:
                    function(dynamic_cast<EntityStatePdu&>(in));
                    break;
                case PDU_FIRE:
                    function(dynamic_cast<FirePdu&>(in));
                    break;
                case PDU_DETONATION:
                    function(dynamic_cast<DetonationPdu&>(in));
                    break;
                case PDU_COLLISION:
                    function(dynamic_cast<CollisionPdu&>(in));
                    break;
                case PDU_SERVICE_REQUEST:
                    function(dynamic_cast<ServiceRequestPdu&>(in));
                    break;
                case PDU_RESUPPLY_OFFER:
                    function(dynamic_cast<ResupplyOfferPdu&>(in));
                    break;
                case PDU_RESUPPLY_RECEIVED:
                    function(dynamic_cast<ResupplyReceivedPdu&>(in));
                    break;
                case PDU_RESUPPLY_CANCEL:
                    function(dynamic_cast<ResupplyCancelPdu&>(in));
                    break;
                case PDU_REPAIR_COMPLETE:
                    function(dynamic_cast<RepairCompletePdu&>(in));
                    break;
                case PDU_REPAIR_RESPONSE:
                    function(dynamic_cast<RepairResponsePdu&>(in));
                    break;
                case PDU_CREATE_ENTITY:
                    function(dynamic_cast<CreateEntityPdu&>(in));
                    break;
                case PDU_REMOVE_ENTITY:
                    function(dynamic_cast<RemoveEntityPdu&>(in));
                    break;
                case PDU_START_RESUME:
                    function(dynamic_cast<StartResumePdu&>(in));
                    break;
                case PDU_STOP_FREEZE:
                    function(dynamic_cast<StopFreezePdu&>(in));
                    break;
                case PDU_ACKNOWLEDGE:
                    function(dynamic_cast<AcknowledgePdu&>(in));
                    break;
                case PDU_ACTION_REQUEST:
                    function(dynamic_cast<ActionRequestPdu&>(in));
                    break;
                case PDU_ACTION_RESPONSE:
                    function(dynamic_cast<ActionResponsePdu&>(in));
                    break;
                case PDU_DATA_QUERY:
                    function(dynamic_cast<DataQueryPdu&>(in));
                    break;
                case PDU_SET_DATA:
                    function(dynamic_cast<SetDataPdu&>(in));
                    break;
                case PDU_DATA:
                    function(dynamic_cast<DataPdu&>(in));
                    break;
                case PDU_EVENT_REPORT:
                    function(dynamic_cast<EventReportPdu&>(in));
                    break;
                case PDU_COMMENT:
                    function(dynamic_cast<CommentPdu&>(in));
                    break;
                case PDU_ELECTRONIC_EMISSIONS:
                    function(dynamic_cast<ElectromagneticEmissionsPdu&>(in));
                    break;
                case PDU_DESIGNATOR:
                    function(dynamic_cast<DesignatorPdu&>(in));
                    break;
                case PDU_TRANSMITTER:
                    function(dynamic_cast<TransmitterPdu&>(in));
                    break;
                case PDU_SIGNAL:
                    function(dynamic_cast<SignalPdu&>(in));
                    break;
                case PDU_RECEIVER:
                    function(dynamic_cast<ReceiverPdu&>(in));
                    break;
                case PDU_IFF:
                    function(dynamic_cast<IffAtcNavAidsLayer1Pdu&>(in));
                    break;
                case PDU_UNDERWATER_ACOUSTIC:
                    function(dynamic_cast<UaPdu&>(in));
                    break;
                case PDU_SUPPLEMENTAL_EMISSION_ENTITY:
                    function(dynamic_cast<SeesPdu&>(in));
                    break;
                case PDU_INTERCOM_SIGNAL:
                    function(dynamic_cast<IntercomSignalPdu&>(in));
                    break;
                case PDU_INTERCOM_CONTROL:
                    function(dynamic_cast<IntercomControlPdu&>(in));
                    break;
                case PDU_AGGREGATE_STATE:
                    function(dynamic_cast<AggregateStatePdu&>(in));
                    break;
                case PDU_ISGROUPOF:
                    function(dynamic_cast<IsGroupOfPdu&>(in));
                    break;
                case PDU_TRANSFER_OWNERSHIP:
                    function(dynamic_cast<TransferControlRequestPdu&>(in));
                    break;
                case PDU_ISPARTOF:
                    function(dynamic_cast<IsPartOfPdu&>(in));
                    break;
                case PDU_MINEFIELD_STATE:
                    function(dynamic_cast<MinefieldStatePdu&>(in));
                    break;
                case PDU_MINEFIELD_QUERY:
                    function(dynamic_cast<MinefieldQueryPdu&>(in));
                    break;
                case PDU_MINEFIELD_DATA:
                    function(dynamic_cast<MinefieldDataPdu&>(in));
                    break;
                case PDU_MINEFIELD_RESPONSE_NACK:
                    function(dynamic_cast<MinefieldResponseNackPdu&>(in));
                    break;
                case PDU_ENVIRONMENTAL_PROCESS:
                    function(dynamic_cast<EnvironmentalProcessPdu&>(in));
                    break;
                case PDU_GRIDDED_DATA:
                    function(dynamic_cast<GriddedDataPdu&>(in));
                    break;
                case PDU_POINT_OBJECT_STATE:
                    function(dynamic_cast<PointObjectStatePdu&>(in));
                    break;
                case PDU_LINEAR_OBJECT_STATE:
                    function(dynamic_cast<LinearObjectStatePdu&>(in));
                    break;
                case PDU_AREAL_OBJECT_STATE:
                    function(dynamic_cast<ArealObjectStatePdu&>(in));
                    break;
                // "reliable" PDUs
                case PDU_CREATE_ENTITY_R:
                    function(dynamic_cast<CreateEntityReliablePdu&>(in));
                    break;
                case PDU_REMOVE_ENTITY_R:
                    function(dynamic_cast<RemoveEntityReliablePdu&>(in));
                    break;
                case PDU_START_RESUME_R:
                    function(dynamic_cast<StartResumeReliablePdu&>(in));
                    break;
                case PDU_STOP_FREEZE_R:
                    function(dynamic_cast<StopFreezeReliablePdu&>(in));
                    break;
                case PDU_ACKNOWLEDGE_R:
                    function(dynamic_cast<AcknowledgeReliablePdu&>(in));
                    break;
                case PDU_ACTION_REQUEST_R:
                    function(dynamic_cast<ActionRequestReliablePdu&>(in));
                    break;
                case PDU_ACTION_RESPONSE_R:
                    function(dynamic_cast<ActionRequestReliablePdu&>(in));
                    break;
                case PDU_DATA_QUERY_R:
                    function(dynamic_cast<DataQueryReliablePdu&>(in));
                    break;
                case PDU_SET_DATA_R:
                    function(dynamic_cast<SetDataReliablePdu&>(in));
                    break;
                case PDU_DATA_R:
                    function(dynamic_cast<DataReliablePdu&>(in));
                    break;
                case PDU_EVENT_REPORT_R:
                    function(dynamic_cast<EventReportReliablePdu&>(in));
                    break;
                case PDU_COMMENT_R:
                    function(dynamic_cast<CommentReliablePdu&>(in));
                    break;
                case PDU_RECORD_QUERY_R:
                    function(dynamic_cast<RecordQueryReliablePdu&>(in));
                    break;
                case PDU_SET_RECORD_R:
                    function(dynamic_cast<SetRecordReliablePdu&>(in));
                    break;
                // back to normal PDUs again.
                case PDU_COLLISION_ELASTIC:
                    function(dynamic_cast<CollisionElasticPdu&>(in));
                    break;
                case PDU_ENTITY_STATE_UPDATE:
                    function(dynamic_cast<EntityStateUpdatePdu&>(in));
                    break;

                // deprecated PDUs
                case PDU_TSPI:
                    throw std::invalid_argument("TSPI PDU unsupported at this time.");
                case PDU_APPEARANCE:
                    throw std::invalid_argument("Appearance PDU unsupported at this time.");
                case PDU_ARTICULATED_PARTS:
                    throw std::invalid_argument("Articulated Parts PDU unsupported at this time.");
                // invalid PDUs
                case PDU_OTHER:
                default:
                    throw std::invalid_argument("Bad PDU type presented of value " + std::to_string(static_cast<long long>(in.getPduType())));
            }
        }
    };   
}

#endif
