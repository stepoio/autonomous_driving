
(cl:in-package :asdf)

(defsystem "platooning-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "acceleration" :depends-on ("_package_acceleration"))
    (:file "_package_acceleration" :depends-on ("_package"))
    (:file "distanceToObj" :depends-on ("_package_distanceToObj"))
    (:file "_package_distanceToObj" :depends-on ("_package"))
    (:file "platoonProtocol" :depends-on ("_package_platoonProtocol"))
    (:file "_package_platoonProtocol" :depends-on ("_package"))
    (:file "steeringAngle" :depends-on ("_package_steeringAngle"))
    (:file "_package_steeringAngle" :depends-on ("_package"))
    (:file "targetAngle" :depends-on ("_package_targetAngle"))
    (:file "_package_targetAngle" :depends-on ("_package"))
    (:file "targetDistance" :depends-on ("_package_targetDistance"))
    (:file "_package_targetDistance" :depends-on ("_package"))
    (:file "targetSpeed" :depends-on ("_package_targetSpeed"))
    (:file "_package_targetSpeed" :depends-on ("_package"))
    (:file "templateMsg" :depends-on ("_package_templateMsg"))
    (:file "_package_templateMsg" :depends-on ("_package"))
    (:file "userInterface" :depends-on ("_package_userInterface"))
    (:file "_package_userInterface" :depends-on ("_package"))
  ))