#pragma once

#include <catboost/libs/helpers/exception.h>

#include <library/cpp/binsaver/bin_saver.h>
#include <library/object_factory/object_factory.h>

#include <util/generic/ptr.h>
#include <util/generic/strbuf.h>
#include <util/generic/string.h>


namespace NCB {

    // Represents '[scheme://]path', empty scheme is ok
    struct TPathWithScheme {
        TString Scheme; // empty is ok
        TString Path;

    public:
        TPathWithScheme() = default;

        explicit TPathWithScheme(TStringBuf pathWithScheme, TStringBuf defaultScheme = "") {
            TStringBuf part1, part2;
            pathWithScheme.Split(AsStringBuf("://"), part1, part2);
            if (part1 == pathWithScheme) { // no scheme in pathWithScheme
                Scheme = defaultScheme;
                Path = part1;
            } else {
                CB_ENSURE(!part1.empty(),
                          "Empty scheme part for path with scheme: " << pathWithScheme);
                Scheme = part1;
                Path = part2;
            }
            CB_ENSURE(!Path.empty(), "Empty path part for path with scheme: " << pathWithScheme);
        }

        SAVELOAD(Scheme, Path);

        bool Inited() const {
            return !Path.empty();
        }
    };


    template <class ISchemeDependentProcessor, class... TArgs>
    THolder<ISchemeDependentProcessor> GetProcessor(TPathWithScheme pathWithScheme, TArgs&&... args) {
        auto res = NObjectFactory::TParametrizedObjectFactory<ISchemeDependentProcessor, TString, TArgs...>::Construct(
                pathWithScheme.Scheme, std::forward<TArgs>(args)...
            );
        CB_ENSURE(res != nullptr,
                  "Processor for scheme [" << pathWithScheme.Scheme << "] not found");
        return res;
    }

}


