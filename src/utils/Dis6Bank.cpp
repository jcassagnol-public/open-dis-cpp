#include <utils/Dis6Bank.h>

#include <nlohmann/json.hpp>

using namespace DIS;

Pdu* Dis6Bank::GetStaticPDU( DIS::PDUType pdu_type )
{
    thread_local AcknowledgePdu acknowledgePdu;
    thread_local AcknowledgeReliablePdu acknowledgeReliablePdu;
    thread_local ActionRequestPdu actionRequestPdu;
    thread_local ActionRequestReliablePdu actionRequestReliablePdu;
    thread_local ActionResponsePdu actionResponsePdu;
    thread_local ActionResponseReliablePdu actionResponseReliablePdu;
    thread_local AggregateStatePdu aggregateStatePdu;
    thread_local ArealObjectStatePdu arealObjectStatePdu;
    thread_local CollisionElasticPdu collisionElasticPdu;
    thread_local CollisionPdu collisionPdu;
    thread_local CommentPdu commentPdu;
    thread_local CommentReliablePdu commentReliablePdu;
    thread_local CreateEntityPdu createEntityPdu;
    thread_local CreateEntityReliablePdu createEntityReliablePdu;
    thread_local DataPdu dataPdu;
    thread_local DataQueryPdu dataQueryPdu;
    thread_local DataQueryReliablePdu dataQueryReliablePdu;
    thread_local DataReliablePdu dataReliablePdu;
    thread_local DesignatorPdu designatorPdu;
    thread_local DetonationPdu detonationPdu;
    thread_local ElectromagneticEmissionsPdu electromagneticEmissionsPdu;
    thread_local EntityStatePdu entityStatePdu;
    thread_local EntityStateUpdatePdu entityStateUpdatePdu;
    thread_local EnvironmentalProcessPdu environmentalProcessPdu;
    thread_local EventReportPdu eventReportPdu;
    thread_local EventReportReliablePdu eventReportReliablePdu;
    thread_local FastEntityStatePdu fastEntityStatePdu;
    thread_local FirePdu firePdu;
    thread_local GriddedDataPdu griddedDataPdu;
    thread_local IffAtcNavAidsLayer1Pdu iffAtcNavAidsLayer1Pdu;
    thread_local IffAtcNavAidsLayer2Pdu iffAtcNavAidsLayer2Pdu;
    thread_local IntercomControlPdu intercomControlPdu;
    thread_local IntercomSignalPdu intercomSignalPdu;
    thread_local IsGroupOfPdu isGroupOfPdu;
    thread_local IsPartOfPdu isPartOfPdu;
    thread_local LinearObjectStatePdu linearObjectStatePdu;
    thread_local LogisticsPdu logisticsPdu;
    thread_local MinefieldDataPdu minefieldDataPdu;
    thread_local MinefieldQueryPdu minefieldQueryPdu;
    thread_local MinefieldResponseNackPdu minefieldResponseNackPdu;
    thread_local MinefieldStatePdu minefieldStatePdu;
    thread_local PointObjectStatePdu pointObjectStatePdu;
    thread_local ReceiverPdu receiverPdu;
    thread_local RecordQueryReliablePdu recordQueryReliablePdu;
    thread_local RemoveEntityPdu removeEntityPdu;
    thread_local RemoveEntityReliablePdu removeEntityReliablePdu;
    thread_local RepairCompletePdu repairCompletePdu;
    thread_local RepairResponsePdu repairResponsePdu;
    thread_local ResupplyCancelPdu resupplyCancelPdu;
    thread_local ResupplyOfferPdu resupplyOfferPdu;
    thread_local ResupplyReceivedPdu resupplyReceivedPdu;
    thread_local SeesPdu seesPdu;
    thread_local ServiceRequestPdu serviceRequestPdu;
    thread_local SetDataPdu setDataPdu;
    thread_local SetDataReliablePdu setDataReliablePdu;
    thread_local SetRecordReliablePdu setRecordReliablePdu;
    thread_local SignalPdu signalPdu;
    thread_local StartResumePdu startResumePdu;
    thread_local StartResumeReliablePdu startResumeReliablePdu;
    thread_local StopFreezePdu stopFreezePdu;
    thread_local StopFreezeReliablePdu stopFreezeReliablePdu;
    thread_local TransferControlRequestPdu transferControlRequestPdu;
    thread_local TransmitterPdu transmitterPdu;
    thread_local UaPdu uaPdu;

    switch (pdu_type)
    {
    case PDU_ENTITY_STATE:
        return &entityStatePdu;
    case PDU_FIRE:
        return &firePdu;
    case PDU_DETONATION:
        return &detonationPdu;
    case PDU_COLLISION:
        return &collisionPdu;
    case PDU_SERVICE_REQUEST:
        return &serviceRequestPdu;
    case PDU_RESUPPLY_OFFER:
        return &resupplyOfferPdu;
    case PDU_RESUPPLY_RECEIVED:
        return &resupplyReceivedPdu;
    case PDU_RESUPPLY_CANCEL:
        return &resupplyCancelPdu;
    case PDU_REPAIR_COMPLETE:
        return &repairCompletePdu;
    case PDU_REPAIR_RESPONSE:
        return &repairResponsePdu;
    case PDU_CREATE_ENTITY:
        return &createEntityPdu;
    case PDU_REMOVE_ENTITY:
        return &removeEntityPdu;
    case PDU_START_RESUME:
        return &startResumePdu;
    case PDU_STOP_FREEZE:
        return &stopFreezePdu;
    case PDU_ACKNOWLEDGE:
        return &acknowledgePdu;
    case PDU_ACTION_REQUEST:
        return &actionRequestPdu;
    case PDU_ACTION_RESPONSE:
        return &actionResponsePdu;
    case PDU_DATA_QUERY:
        return &dataQueryPdu;
    case PDU_SET_DATA:
        return &setDataPdu;
    case PDU_DATA:
        return &dataPdu;
    case PDU_EVENT_REPORT:
        return &eventReportPdu;
    case PDU_COMMENT:
        return &commentPdu;
    case PDU_ELECTRONIC_EMISSIONS:
        return &electromagneticEmissionsPdu;
    case PDU_DESIGNATOR:
        return &designatorPdu;
    case PDU_TRANSMITTER:
        return &transmitterPdu;
    case PDU_SIGNAL:
        return &signalPdu;
    case PDU_RECEIVER:
        return &receiverPdu;
    case PDU_IFF:
        return &iffAtcNavAidsLayer1Pdu;
    case PDU_UNDERWATER_ACOUSTIC:
        return &uaPdu;
    case PDU_SUPPLEMENTAL_EMISSION_ENTITY:
        return &seesPdu;
    case PDU_INTERCOM_SIGNAL:
        return &intercomSignalPdu;
    case PDU_INTERCOM_CONTROL:
        return &intercomControlPdu;
    case PDU_AGGREGATE_STATE:
        return &aggregateStatePdu;
    case PDU_ISGROUPOF:
        return &isGroupOfPdu;
    case PDU_TRANSFER_OWNERSHIP:
        return &transferControlRequestPdu;
    case PDU_ISPARTOF:
        return &isPartOfPdu;
    case PDU_MINEFIELD_STATE:
        return &minefieldStatePdu;
    case PDU_MINEFIELD_QUERY:
        return &minefieldQueryPdu;
    case PDU_MINEFIELD_DATA:
        return &minefieldDataPdu;
    case PDU_MINEFIELD_RESPONSE_NACK:
        return &minefieldResponseNackPdu;
    case PDU_ENVIRONMENTAL_PROCESS:
        return &environmentalProcessPdu;
    case PDU_GRIDDED_DATA:
        return &griddedDataPdu;
    case PDU_POINT_OBJECT_STATE:
        return &pointObjectStatePdu;
    case PDU_LINEAR_OBJECT_STATE:
        return &linearObjectStatePdu;
    case PDU_AREAL_OBJECT_STATE:
        return &arealObjectStatePdu;
    // "reliable" PDUs
    case PDU_CREATE_ENTITY_R:
        return &createEntityReliablePdu;
    case PDU_REMOVE_ENTITY_R:
        return &removeEntityReliablePdu;
    case PDU_START_RESUME_R:
        return &startResumeReliablePdu;
    case PDU_STOP_FREEZE_R:
        return &stopFreezeReliablePdu;
    case PDU_ACKNOWLEDGE_R:
        return &acknowledgeReliablePdu;
    case PDU_ACTION_REQUEST_R:
        return &actionRequestReliablePdu;
    case PDU_ACTION_RESPONSE_R:
        return &actionRequestReliablePdu;
    case PDU_DATA_QUERY_R:
        return &dataQueryReliablePdu;
    case PDU_SET_DATA_R:
        return &setDataReliablePdu;
    case PDU_DATA_R:
        return &dataReliablePdu;
    case PDU_EVENT_REPORT_R:
        return &eventReportReliablePdu;
    case PDU_COMMENT_R:
        return &commentReliablePdu;
    case PDU_RECORD_QUERY_R:
        return &recordQueryReliablePdu;
    case PDU_SET_RECORD_R:
        return &setRecordReliablePdu;
    // back to normal PDUs again.
    case PDU_COLLISION_ELASTIC:
        return &collisionElasticPdu;
    case PDU_ENTITY_STATE_UPDATE:
        return &entityStateUpdatePdu;

    // deprecated PDUs
    case PDU_TSPI:
    case PDU_APPEARANCE:
    case PDU_ARTICULATED_PARTS:
    // invalid PDUs
    case PDU_OTHER:
    default:
        throw std::invalid_argument("Bad PDU type presented of value " + std::to_string(static_cast<long long>(pdu_type)));
    }
    return nullptr;
}

std::unique_ptr<Pdu> Dis6Bank::GetPDUFromJSON(const nlohmann::json& in_json)
{
    if (!in_json.is_object())
    {
        throw std::invalid_argument("Can only cast object JSON to PDUs");
    }
    unsigned char wire_pdu_type = 0;
    in_json.at("pduType").get_to(wire_pdu_type);
    DIS::PDUType casted_type = static_cast<DIS::PDUType>(wire_pdu_type);
    Pdu* constructor = GetStaticPDU(casted_type);
    if (!constructor)
    {
        throw std::invalid_argument("Bad PDU type " + std::to_string(static_cast<long long>(wire_pdu_type)));
    }

    std::unique_ptr<Pdu> pdu_copy;

    auto cast_all_classes_from_json = [&in_json, &pdu_copy](auto &in_class)
    {
        from_json(in_json, in_class);
        pdu_copy = std::make_unique<Pdu>(in_class);
    };
    parseObjectGivenType(*constructor, cast_all_classes_from_json);
    return pdu_copy;
}

nlohmann::json Dis6Bank::GetJSONFromPDU(Pdu& in)
{
    nlohmann::json to_return;
    auto cast_all_classes_to_json = [&to_return, &in](auto &in_class)
    {
        to_json(to_return, in_class);
    };
    parseObjectGivenType(in, cast_all_classes_to_json);
    return to_return;
}