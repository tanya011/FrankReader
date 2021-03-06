#ifndef YAFR_SERVERPROBLEMSEXCEPTION_H
#define YAFR_SERVERPROBLEMSEXCEPTION_H

#include <exception>

struct ServerProblemsExceptionAbort : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "abort";
    }
};

struct ServerProblemsExceptionReconnect : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "reconnect";
    }
};

struct ServerProblemsExceptionNotUploadLibrary : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "not upload library";
    }
};

struct ServerProblemsExceptionNotAddInCollection : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "not add in collection";
    }
};

struct ServerProblemsExceptionNotDeleteInCollection : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "not delete in collection";
    }
};

struct ServerProblemsExceptionNotSaveDict : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "not save in dict";
    }
};

#endif //YAFR_SERVERPROBLEMSEXCEPTION_H
