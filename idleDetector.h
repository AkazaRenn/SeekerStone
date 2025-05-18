#include <QGamepad>
#include <QGamepadManager>
#include <memory>

class IdleDetector : public QObject {
        Q_OBJECT
    public:
        explicit IdleDetector(QObject *parent = nullptr);
        ~IdleDetector();

    protected:
        // Event filter to catch all input events
        bool eventFilter(QObject *watched, QEvent *event) override;

    private:
        void onGamepadConnected();
        void resetIdleTimer();
};
