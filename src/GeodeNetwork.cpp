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
        _listener.spawn(
            req.get(_url),
            [this](web::WebResponse result) {
                _data = result.string().unwrapOr("Not a string");

                if (result.ok() && _onOk != nullptr) {
                    _onOk(this);
                } else if (!result.ok() && _onError != nullptr) {
                    _onError(this);
                }
            }
        );
    }
}

GeodeNetwork::GeodeNetwork() {}
