#include "GeodeNetwork.hpp"
using namespace geode::prelude;

void GeodeNetwork::setOkCallback(std::function<void(GeodeNetwork *)> ok) {
    _onOk = ok;
}
void GeodeNetwork::setErrorCallback(std::function<void(GeodeNetwork *)> error) {
    _onError = error;
}

void GeodeNetwork::setURL(std::string url) {
    _url = url;
}
void GeodeNetwork::setMethod(HttpMethod method) {
    _method = method;
}

std::string &GeodeNetwork::getResponse() {
    return _data;
}

void GeodeNetwork::send() {
    web::WebRequest req;
    req.timeout(std::chrono::seconds(10));

    if (_method == MGet) {
        _listener.setFilter(req.get(_url));
    }
}

GeodeNetwork::GeodeNetwork() {
    _listener.bind([this](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            _data = res->string().unwrapOr("Not a string");

            if (res->ok() && _onOk != nullptr) {
                _onOk(this);
            } else if (!res->ok() && _onError != nullptr) {
                _onError(this);
            }
        } else if (e->isCancelled()) {
            _data = "Error: cancelled";
            if (_onError != nullptr) {
                _onError(this);
            }
        }
    });
}
