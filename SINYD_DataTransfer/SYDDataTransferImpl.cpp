#include "SYDDataTransferImpl.h"
#include "JsonRW.h"
#include "FlumeDataRW.h"
#include "TransDataReader_Flume.h"


CSYDDataTransferImpl::CSYDDataTransferImpl()
	: m_rawThread()
	, m_measureThread()
	, m_detailThread()
{
}


CSYDDataTransferImpl::~CSYDDataTransferImpl()
{
}

void CSYDDataTransferImpl::Init(const SourceInfo & info)
{
	CJsonRW::initSourceInfo(info);

    std::tr1::shared_ptr<ITransDataReader> raw_reader(new CRawDataReader());
    std::tr1::shared_ptr<IReturnCodeExecutor> raw_ret_exec(new CReturnCodeExecutor());
    std::tr1::shared_ptr<ISocketWorker> raw_worker(new CSocketWorker(raw_reader, raw_ret_exec));
	m_rawThread.reset(new CSocketThread(raw_worker));

    std::tr1::shared_ptr<ITransDataReader> measure_reader(new CMeasureDataReader());
    std::tr1::shared_ptr<IReturnCodeExecutor> measure_ret_exec(new CReturnCodeExecutor());
    std::tr1::shared_ptr<ISocketWorker> measure_worker(new CSocketWorker(measure_reader, measure_ret_exec));
	m_measureThread.reset(new CSocketThread(measure_worker));

    std::tr1::shared_ptr<ITransDataReader> detail_reader(new CDetailDataReader());
    std::tr1::shared_ptr<IReturnCodeExecutor> detail_ret_exec(new CReturnCodeExecutor());
    std::tr1::shared_ptr<ISocketWorker> detail_worker(new CSocketWorker(detail_reader, detail_ret_exec));
	m_detailThread.reset(new CSocketThread(detail_worker));
}

void CSYDDataTransferImpl::Init2(const SourceInfo2 & info)
{
    std::tr1::shared_ptr<ITransDataReader> raw_reader;
    std::tr1::shared_ptr<ITransDataReader> measure_reader;
    std::tr1::shared_ptr<ITransDataReader> detail_reader;
    std::tr1::shared_ptr<IReturnCodeExecutor> raw_ret_exec;
    std::tr1::shared_ptr<IReturnCodeExecutor> measure_ret_exec;
    std::tr1::shared_ptr<IReturnCodeExecutor> detail_ret_exec;

	TransferFormatType type = CTransConfig::GetTransFormatType(TYPE_FLUME);
	if (type == TYPE_FLUME)
	{
		raw_reader.reset(new CRawDataReader_Flume());
		measure_reader.reset(new CMeasureDataReader_Flume());
		detail_reader.reset(new CDetailDataReader_Flume());

		raw_ret_exec.reset(new CReturnCodeExecutor_Flume());
		measure_ret_exec.reset(new CReturnCodeExecutor_Flume());
		detail_ret_exec.reset(new CReturnCodeExecutor_Flume());

		CFlumeDataRW::initSourceInfo(info);
	}
	else if (type == TYPE_NORMAL)
	{
		measure_reader.reset(new CMeasureDataReader());
		raw_reader.reset(new CRawDataReader());
		detail_reader.reset(new CDetailDataReader());

		raw_ret_exec.reset(new CReturnCodeExecutor());
		measure_ret_exec.reset(new CReturnCodeExecutor());
		detail_ret_exec.reset(new CReturnCodeExecutor());

		SourceInfo info_normal;
		info_normal.mix_id = atoi(info.mix_machine_id);
		info_normal.mix_type = info.mix_type;
		info_normal.source_type = info.source_type;
		CJsonRW::initSourceInfo(info_normal);
	}
	else
	{
		return;
	}

    std::tr1::shared_ptr<ISocketWorker> raw_worker(new CSocketWorker(raw_reader, raw_ret_exec));
	m_rawThread.reset(new CSocketThread(raw_worker));

    std::tr1::shared_ptr<ISocketWorker> measure_worker(new CSocketWorker(measure_reader, measure_ret_exec));
	m_measureThread.reset(new CSocketThread(measure_worker));

    std::tr1::shared_ptr<ISocketWorker> detail_worker(new CSocketWorker(detail_reader, detail_ret_exec));
	m_detailThread.reset(new CSocketThread(detail_worker));
}

void CSYDDataTransferImpl::Start(const char* ip, int port)
{
	if (m_rawThread)
	{
		m_rawThread->Start(ip, port);
	}
	if (m_measureThread)
	{
		m_measureThread->Start(ip, port);
	}
	if (m_detailThread)
	{
		m_detailThread->Start(ip, port);
	}

}

void CSYDDataTransferImpl::Stop()
{
	if (m_rawThread)
	{
		m_rawThread->Stop();
	}
	if (m_measureThread)
	{
		m_measureThread->Stop();
	}
	if (m_detailThread)
	{
		m_detailThread->Stop();
	}
}
