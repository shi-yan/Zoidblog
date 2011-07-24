#include "httprequest.h"

HttpRequest::HttpRequest():QObject(),header(),hasSetFormData(false),formData()
{
}

HttpRequest::HttpRequest(const HttpRequest &in):QObject(),header(in.header),debugInfo(in.debugInfo),hasSetFormData(in.hasSetFormData),formData(in.formData)
{
}

void HttpRequest::operator=(const HttpRequest &in)
{
    header=in.header;
    debugInfo=in.debugInfo;
    hasSetFormData=in.hasSetFormData;
    formData=in.formData;
}


HttpRequest::~HttpRequest()
{

}
