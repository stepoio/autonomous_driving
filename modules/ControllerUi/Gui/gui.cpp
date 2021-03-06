#include "gui.h"
#include "ui_gui.h"

ControllerUi::ControllerUi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControllerUi)
{
    ui->setupUi(this);

    //start server
    recv_udp_msg_cb = boost::bind( boost::mem_fn(&ControllerUi::receive_message), this, _1 );

    server_ptr_ = std::unique_ptr<UdpServer>( new UdpServer( recv_udp_msg_cb
                                                  , udp::endpoint(udp::v4(),13500)
                                                  , udp::endpoint(boost::asio::ip::address_v4::broadcast(),13500)));

    server_ptr_->set_filter_own_broadcasts(true);

    //useful default platooning shit
    ui->info_set_lvfv_v1->setText("LV");
    ui->info_set_platoonspeed_v1->setText("3");
    ui->info_set_platoon_distance_v1->setText("1");
    ui->info_set_lvfv_v2->setText("FV");
    ui->info_set_platoonspeed_v2->setText("3");
    ui->info_set_platoon_distance_v2->setText("1");
    ui->info_set_lvfv_v3->setText("FV");
    ui->info_set_platoonspeed_v3->setText("3");
    ui->info_set_platoon_distance_v3->setText("1");

    std::cerr << "controllerUI LOADED" << std::endl;
}

ControllerUi::~ControllerUi()
{
    delete ui;
}

void ControllerUi::set_platooningbutton_v1( const bool& enabled ) {

    platooningEnabled_v1_ = enabled;

    if( enabled ) {
        ui->togglePlatooning_v1->setStyleSheet("background-color: red");
    } else {
        ui->togglePlatooning_v1->setStyleSheet("background-color: grey");
    }

}
void ControllerUi::set_platooningbutton_v2( const bool& enabled ) {
    platooningEnabled_v2_ = enabled;

    if( enabled ) {
        ui->togglePlatooning_v2->setStyleSheet("background-color: red");
    } else {
        ui->togglePlatooning_v2->setStyleSheet("background-color: grey");
    }
}
void ControllerUi::set_platooningbutton_v3( const bool& enabled ) {
    platooningEnabled_v3_ = enabled;

    if( enabled ) {
        ui->togglePlatooning_v3->setStyleSheet("background-color: red");
    } else {
        ui->togglePlatooning_v3->setStyleSheet("background-color: grey");
    }
}

void ControllerUi::on_togglePlatooning_v1_clicked()
{
    try {
        platooningEnabled_v1_ = !platooningEnabled_v1_;
        set_platooningbutton_v1(platooningEnabled_v1_);
        float ipd;
        float ps;
        std::string lvfv;

        if( platooningEnabled_v1_ ) {
            if( ui->info_set_platoon_distance_v1->text() == "") {
                platooningEnabled_v1_ = !platooningEnabled_v1_;
                QMessageBox msgBox;
                msgBox.setText("IPD cannot be empty.");
                msgBox.exec();
                return;
            }

            try {
                ipd = std::stof(ui->info_set_platoon_distance_v1->text().toStdString() );
            } catch( std::invalid_argument &ex ) {
                platooningEnabled_v1_ = !platooningEnabled_v1_;
                QMessageBox msgBox;
                msgBox.setText("IPD cannot be whatever you wrote in it.");
                msgBox.exec();
                return;
            }

            if( ui->info_set_platoonspeed_v1->text() == "") {
                platooningEnabled_v1_ = !platooningEnabled_v1_;
                QMessageBox msgBox;
                msgBox.setText("PS cannot be empty.");
                msgBox.exec();
                return;
            }

            try {
                ps = std::stof(ui->info_set_platoonspeed_v1->text().toStdString() );
            } catch( std::invalid_argument &ex ) {
                platooningEnabled_v1_ = !platooningEnabled_v1_;
                QMessageBox msgBox;
                msgBox.setText("PS cannot be whatever you wrote in it.");
                msgBox.exec();
                return;
            }

            if( ui->info_set_lvfv_v1->text() == ""
                    || ( ui->info_set_lvfv_v1->text() != "LV" && ui->info_set_lvfv_v1->text() != "FV" )) {
                platooningEnabled_v1_ = !platooningEnabled_v1_;
                QMessageBox msgBox;
                msgBox.setText("LVFV must be set.");
                msgBox.exec();
                return;
            }
        }

        platooning::platooningToggle msg;
        msg.enable_platooning = platooningEnabled_v1_;
        msg.vehicle_id = 1;
        msg.inner_platoon_distance = ipd;
        msg.platoon_speed = ps;
        msg.lvfv = ui->info_set_lvfv_v1->text().toStdString();
        server_ptr_->start_send(platooning::MessageTypes::encode_message(msg),REMOTE_PLATOONINGTOGGLE);

    } catch( std::exception &ex ) {
        std::cerr << "toggleplatooning crash with " << ex.what() << std::endl;
    }
}

void ControllerUi::on_togglePlatooning_v2_clicked()
{
    try {
        platooningEnabled_v2_ = !platooningEnabled_v2_;
        set_platooningbutton_v2(platooningEnabled_v2_);
        float ipd;
        float ps;
        std::string lvfv = "";

        if( platooningEnabled_v2_ ) {
            if( ui->info_set_platoonspeed_v2->text() == "") {
                platooningEnabled_v2_ = !platooningEnabled_v2_;
                QMessageBox msgBox;
                msgBox.setText("IPD cannot be empty.");
                msgBox.exec();
                return;
            }

            try {
                ipd = std::stof(ui->info_set_platoonspeed_v2->text().toStdString() );
            } catch( std::invalid_argument &ex ) {
                platooningEnabled_v2_ = !platooningEnabled_v2_;
                QMessageBox msgBox;
                msgBox.setText("IPD cannot be whatever you wrote in it.");
                msgBox.exec();
                return;
            }

            if( ui->info_set_platoon_distance_v2->text() == "") {
                platooningEnabled_v2_ = !platooningEnabled_v2_;
                QMessageBox msgBox;
                msgBox.setText("PS cannot be empty.");
                msgBox.exec();
                return;
            }

            try {
                ps = std::stof(ui->info_set_platoonspeed_v2->text().toStdString() );
            } catch( std::invalid_argument &ex ) {
                platooningEnabled_v2_ = !platooningEnabled_v2_;
                QMessageBox msgBox;
                msgBox.setText("PS cannot be whatever you wrote in it.");
                msgBox.exec();
                return;
            }

            if( ui->info_set_lvfv_v2->text() == ""
                    || ( ui->info_set_lvfv_v2->text() != "LV" && ui->info_set_lvfv_v2->text() != "FV" )) {
                platooningEnabled_v2_ = !platooningEnabled_v2_;
                QMessageBox msgBox;
                msgBox.setText("LVFV must be set.");
                msgBox.exec();
                return;
            }
        }

        platooning::platooningToggle msg;
        msg.enable_platooning = platooningEnabled_v2_;
        msg.vehicle_id = 2;
        msg.inner_platoon_distance = ipd;
        msg.platoon_speed = ps;
        msg.lvfv = ui->info_set_lvfv_v2->text().toStdString();
        server_ptr_->start_send(platooning::MessageTypes::encode_message(msg),REMOTE_PLATOONINGTOGGLE);

    } catch( std::exception &ex ) {
        std::cerr << "toggleplatooning crash with " << ex.what() << std::endl;
    }

}

void ControllerUi::on_togglePlatooning_v3_clicked()
{
    try {
        platooningEnabled_v3_ = !platooningEnabled_v3_;
        set_platooningbutton_v3(platooningEnabled_v3_);
        float ipd;
        float ps;
        std::string lvfv = "";

        if( platooningEnabled_v3_ ) {
            if( ui->info_set_platoonspeed_v3->text() == "") {
                platooningEnabled_v3_ = !platooningEnabled_v3_;
                QMessageBox msgBox;
                msgBox.setText("IPD cannot be empty.");
                msgBox.exec();
                return;
            }

            try {
                ipd = std::stof(ui->info_set_platoonspeed_v3->text().toStdString() );
            } catch( std::invalid_argument &ex ) {
                platooningEnabled_v3_ = !platooningEnabled_v3_;
                QMessageBox msgBox;
                msgBox.setText("IPD cannot be whatever you wrote in it.");
                msgBox.exec();
                return;
            }

            if( ui->info_set_platoon_distance_v3->text() == "") {
                platooningEnabled_v3_ = !platooningEnabled_v3_;
                QMessageBox msgBox;
                msgBox.setText("PS cannot be empty.");
                msgBox.exec();
                return;
            }

            try {
                ps = std::stof(ui->info_set_platoonspeed_v3->text().toStdString() );
            } catch( std::invalid_argument &ex ) {
                platooningEnabled_v3_ = !platooningEnabled_v3_;
                QMessageBox msgBox;
                msgBox.setText("PS cannot be whatever you wrote in it.");
                msgBox.exec();
                return;
            }

            if( ui->info_set_lvfv_v3->text() == ""
                    || ( ui->info_set_lvfv_v3->text() != "LV" && ui->info_set_lvfv_v3->text() != "FV" )) {
                platooningEnabled_v3_ = !platooningEnabled_v3_;
                QMessageBox msgBox;
                msgBox.setText("LVFV must be set.");
                msgBox.exec();
                return;
            }
        }

        platooning::platooningToggle msg;
        msg.enable_platooning = platooningEnabled_v3_;
        msg.vehicle_id = 3;
        msg.inner_platoon_distance = ipd;
        msg.platoon_speed = ps;
        msg.lvfv = ui->info_set_lvfv_v3->text().toStdString();
        server_ptr_->start_send(platooning::MessageTypes::encode_message(msg),REMOTE_PLATOONINGTOGGLE);

    } catch( std::exception &ex ) {
        std::cerr << "toggleplatooning crash with " << ex.what() << std::endl;
    }

}

void ControllerUi::add_slave_vehicle( uint32_t vehicle_id) {
    slave_vehicle_ids_.emplace_back(vehicle_id);
}

void ControllerUi::receive_message( boost::shared_ptr<std::pair<std::string, uint32_t>> msgpair )
{

    try {
        if( msgpair->first.empty()) {
            return;
        }

        platooning::userInterface msg;

        switch( msgpair->second ) {
            case REMOTE_USERINTERFACE:

            msg = platooning::MessageTypes::decode_json<platooning::userInterface>(msgpair->first);

            std::cout << msgpair->second << std::endl << msgpair->first << std::endl;

            if( msg.src_vehicle == 1 ) {
                try { ui->info_actual_distance_v1->setText( std::to_string(msg.actual_distance).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_actual_distance_v1" << std::endl;}
                try { ui->info_actual_speed_v1->setText( std::to_string(msg.speed).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_actual_speed_v1" << std::endl;}
                try { ui->info_ipd_v1->setText( std::to_string(msg.inner_platoon_distance).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_ipd_v1" << std::endl;}
                try { ui->info_lvfv_v1->setText( msg.following_vehicle ? "FV" : "LV" );} catch( std::exception &ex ) { std::cerr << "error info_lvfv_v1" << std::endl;}
                try { ui->info_lvfv_v1->setText( msg.leading_vehicle ? "LV" : "FV" );} catch( std::exception &ex ) { std::cerr << "error info_lvfv_v1" << std::endl;}
                try { ui->info_platooningstate_v1->setText( msg.platooning_state.c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platooningstate_v1" << std::endl;}
                try { ui->info_platoonsize_v1->setText( std::to_string(msg.platoon_size).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platoonsize_v1" << std::endl;}
                try { ui->info_ps_v1->setText( std::to_string(msg.platoon_speed).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_ps_v1" << std::endl;}
                try { ui->info_vehicle_id_v1->setText( std::to_string(msg.src_vehicle).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_vehicle_id_v1" << std::endl;}
                try { ui->info_platoonid_v1->setText( std::to_string(msg.platoon_id).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platoonid_v1" << std::endl;}

                try { std::stringstream followerlist;
                    std::copy(msg.platoon_members.begin(), msg.platoon_members.end(), std::ostream_iterator<uint32_t>(followerlist, " "));
                        ui->info_platoonmembers_v1->setText( followerlist.str().c_str());}catch( std::exception &ex )  { std::cerr << "error info_platoonmembers_v1" << std::endl;}

                if( msg.platooning_state != "CREATING" && msg.platooning_state != "RUNNING") {
                    set_platooningbutton_v1(false);
                } else {
                    set_platooningbutton_v1(true);
                }

                std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(p);
                try { ui->info_timestamp_v1->setText( std::ctime(&t) );} catch( std::exception &ex ) { std::cerr << "error info_timestamp_v1" << std::endl;}
            }

            if( msg.src_vehicle == 2 ) {
                try { ui->info_actual_distance_v2->setText( std::to_string(msg.actual_distance).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_actual_distance_v2" << std::endl;}
                try { ui->info_actual_speed_v2->setText( std::to_string(msg.speed).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_actual_speed_v2" << std::endl;}
                try { ui->info_ipd_v2->setText( std::to_string(msg.inner_platoon_distance).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_ipd_v2" << std::endl;}
                try { ui->info_lvfv_v2->setText( msg.following_vehicle ? "FV" : "LV" );} catch( std::exception &ex ) { std::cerr << "error info_lvfv_v2" << std::endl;}
                try { ui->info_lvfv_v2->setText( msg.leading_vehicle ? "LV" : "FV" );} catch( std::exception &ex ) { std::cerr << "error info_lvfv_v2" << std::endl;}
                try { ui->info_platooningstate_v2->setText( msg.platooning_state.c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platooningstate_v2" << std::endl;}
                try { ui->info_platoonsize_v2->setText( std::to_string(msg.platoon_size).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platoonsize_v2" << std::endl;}
                try { ui->info_ps_v2->setText( std::to_string(msg.platoon_speed).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_ps_v2" << std::endl;}
                try { ui->info_vehicle_id_v2->setText( std::to_string(msg.src_vehicle).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_vehicle_id_v2" << std::endl;}
                try { ui->info_platoonid_v2->setText( std::to_string(msg.platoon_id).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platoonid_v2" << std::endl;}

                try { std::stringstream followerlist;
                    std::copy(msg.platoon_members.begin(), msg.platoon_members.end(), std::ostream_iterator<uint32_t>(followerlist, " "));
                        ui->info_platoonmembers_v2->setText( followerlist.str().c_str());}catch( std::exception &ex ) { std::cerr << "error info_platoonmembers_v2" << std::endl;}

                if( msg.platooning_state != "CREATING" && msg.platooning_state != "RUNNING") {
                    set_platooningbutton_v2(false);
                } else {
                    set_platooningbutton_v2(true);
                }

                std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(p);
                try { ui->info_timestamp_v2->setText( std::ctime(&t) );} catch( std::exception &ex ) { std::cerr << "error info_timestamp_v2" << std::endl;}
            }

            if( msg.src_vehicle == 3 ) {
                try { ui->info_actual_distance_v3->setText( std::to_string(msg.actual_distance).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_actual_distance_v3" << std::endl;}
                try { ui->info_actual_speed_v3->setText( std::to_string(msg.speed).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_actual_speed_v3" << std::endl;}
                try { ui->info_ipd_v3->setText( std::to_string(msg.inner_platoon_distance).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_ipd_v3" << std::endl;}
                try { ui->info_lvfv_v3->setText( msg.following_vehicle ? "FV" : "LV" );} catch( std::exception &ex ) { std::cerr << "error info_lvfv_v3" << std::endl;}
                try { ui->info_lvfv_v3->setText( msg.leading_vehicle ? "LV" : "FV" );} catch( std::exception &ex ) { std::cerr << "error info_lvfv_v3" << std::endl;}
                try { ui->info_platooningstate_v3->setText( msg.platooning_state.c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platooningstate_v3" << std::endl;}
                try { ui->info_platoonsize_v3->setText( std::to_string(msg.platoon_size).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platoonsize_v3" << std::endl;}
                try { ui->info_ps_v3->setText( std::to_string(msg.platoon_speed).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_ps_v3" << std::endl;}
                try { ui->info_vehicle_id_v3->setText( std::to_string(msg.src_vehicle).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_vehicle_id_v3" << std::endl;}
                try { ui->info_platoonid_v3->setText( std::to_string(msg.platoon_id).c_str() );} catch( std::exception &ex ) { std::cerr << "error info_platoonid_v3" << std::endl;}

                try { std::stringstream followerlist;
                    std::copy(msg.platoon_members.begin(), msg.platoon_members.end(), std::ostream_iterator<uint32_t>(followerlist, " "));
                        ui->info_platoonmembers_v3->setText( followerlist.str().c_str());}catch( std::exception &ex ) { std::cerr << "error info_platoonmembers_v3" << std::endl;}

                if( msg.platooning_state != "CREATING" && msg.platooning_state != "RUNNING") {
                    set_platooningbutton_v3(false);
                } else {
                    set_platooningbutton_v3(true);
                }

                std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(p);
                try { ui->info_timestamp_v3->setText( std::ctime(&t) );} catch( std::exception &ex ) { std::cerr << "error info_timestamp_v3" << std::endl;}
            }

            break;
        default:
            break;
        }

        //std::find(slave_vehicle_ids_.begin(), slave_vehicle_ids_.end(), remotevehicle_id) != slave_vehicle_ids_.end();


    } catch( std::exception &ex ) {
        std::cerr << "receive_message crash with " << ex.what() << std::endl;
    }
}

void ControllerUi::set_remotecontrolbutton_v1( const bool& enabled ) {

    if( enabled ) {
        remoteEnabled_v1_ = true;
        QWidget::grabKeyboard(); //only this widget get keypresses
        ui->toggleRemote_v1->setStyleSheet("background-color: red");
        ui->cursorDown_v1->setFlat(false);
        ui->cursorUp_v1->setFlat(false);
        ui->cursorLeft_v1->setFlat(false);
        ui->cursorRight_v1->setFlat(false);
        ui->info_remote_lat_v1->setDisabled(false);
        ui->info_remote_speed_v1->setDisabled(false);
        keypollthread_ = boost::thread([this] {
            while(remoteEnabled_v1_) {
                keypresspoll();
            }
        });
    } else {
        QWidget::releaseKeyboard(); //only this widget get keypresses
        ui->toggleRemote_v1->setStyleSheet("background-color: lightgrey");
        ui->cursorDown_v1->setFlat(true);
        ui->cursorUp_v1->setFlat(true);
        ui->cursorLeft_v1->setFlat(true);
        ui->cursorRight_v1->setFlat(true);
        ui->info_remote_lat_v1->setText("");
        ui->info_remote_speed_v1->setText("");
        ui->info_remote_lat_v1->setDisabled(true);
        ui->info_remote_speed_v1->setDisabled(true);
    }
}



void ControllerUi::set_remotecontrolbutton_v2( const bool& enabled ) {
    if( enabled ) {
        remoteEnabled_v2_ = true;
        QWidget::grabKeyboard(); //only this widget get keypresses
        ui->toggleRemote_v2->setStyleSheet("background-color: red");
        ui->cursorDown_v2->setFlat(false);
        ui->cursorUp_v2->setFlat(false);
        ui->cursorLeft_v2->setFlat(false);
        ui->cursorRight_v2->setFlat(false);
        ui->info_remote_lat_v2->setDisabled(false);
        ui->info_remote_speed_v2->setDisabled(false);
        keypollthread_ = boost::thread([this] {
            while(remoteEnabled_v2_) {
                keypresspoll();
            }
        });
    } else {
        remoteEnabled_v2_ = false;
        QWidget::releaseKeyboard(); //only this widget get keypresses
        ui->toggleRemote_v2->setStyleSheet("background-color: lightgrey");
        ui->cursorDown_v2->setFlat(true);
        ui->cursorUp_v2->setFlat(true);
        ui->cursorLeft_v2->setFlat(true);
        ui->cursorRight_v2->setFlat(true);
        ui->info_remote_lat_v2->setText("");
        ui->info_remote_speed_v2->setText("");
        ui->info_remote_lat_v2->setDisabled(true);
        ui->info_remote_speed_v2->setDisabled(true);
    }
}

void ControllerUi::set_remotecontrolbutton_v3( const bool& enabled ) {
    if( enabled ) {
        remoteEnabled_v3_ = true;
        QWidget::grabKeyboard(); //only this widget get keypresses
        ui->toggleRemote_v3->setStyleSheet("background-color: red");
        ui->cursorDown_v3->setFlat(false);
        ui->cursorUp_v3->setFlat(false);
        ui->cursorLeft_v3->setFlat(false);
        ui->cursorRight_v3->setFlat(false);
        ui->info_remote_lat_v3->setDisabled(false);
        ui->info_remote_speed_v3->setDisabled(false);
        keypollthread_ = boost::thread([this] {
            while(remoteEnabled_v3_) {
                keypresspoll();
            }
        });
    } else {
        remoteEnabled_v3_ = false;
        QWidget::releaseKeyboard(); //only this widget get keypresses
        ui->toggleRemote_v3->setStyleSheet("background-color: lightgrey");
        ui->cursorDown_v3->setFlat(true);
        ui->cursorUp_v3->setFlat(true);
        ui->cursorLeft_v3->setFlat(true);
        ui->cursorRight_v3->setFlat(true);
        ui->info_remote_lat_v3->setText("");
        ui->info_remote_speed_v3->setText("");
        ui->info_remote_lat_v3->setDisabled(true);
        ui->info_remote_speed_v3->setDisabled(true);
    }
}

void ControllerUi::on_toggleRemote_v1_clicked()
{
    try {
        remoteEnabled_v1_ = !remoteEnabled_v1_;
        set_remotecontrolbutton_v1( remoteEnabled_v1_);

        platooning::remotecontrolToggle msg;
        msg.enable_remotecontrol = remoteEnabled_v1_;
        server_ptr_->start_send(platooning::MessageTypes::encode_message(msg), REMOTE_CONTROLTOGGLE);

    } catch( std::exception &ex) {
        std::cerr << "toggleremote crash with " << ex.what() << std::endl;
    }

}

void ControllerUi::on_toggleRemote_v2_clicked()
{
    try {
        remoteEnabled_v2_ = !remoteEnabled_v2_;
        set_remotecontrolbutton_v2( remoteEnabled_v2_);

        platooning::remotecontrolToggle msg;
        msg.enable_remotecontrol = remoteEnabled_v2_;
        server_ptr_->start_send(platooning::MessageTypes::encode_message(msg), REMOTE_CONTROLTOGGLE);

    } catch( std::exception &ex) {
        std::cerr << "toggleremote crash with " << ex.what() << std::endl;
    }

}

void ControllerUi::on_toggleRemote_v3_clicked()
{
    try {
        remoteEnabled_v3_ = !remoteEnabled_v3_;
        set_remotecontrolbutton_v3( remoteEnabled_v3_);

        platooning::remotecontrolToggle msg;
        msg.enable_remotecontrol = remoteEnabled_v3_;
        server_ptr_->start_send(platooning::MessageTypes::encode_message(msg), REMOTE_CONTROLTOGGLE);

    } catch( std::exception &ex) {
        std::cerr << "toggleremote crash with " << ex.what() << std::endl;
    }

}

void ControllerUi::keypresspoll() {
    try {
        boost::property_tree::ptree root;
        std::stringstream os;

        int tabix = ui->tabWidget->currentIndex();

        while(remoteEnabled_v1_ && tabix == 0) {
            if(ui->cursorUp_v1->isDown() && remote_speed_v1_ < 2.8) {
                remote_speed_v1_ += 0.1f;

                ui->info_remote_speed_v1->insert( std::to_string( remote_speed_v1_).c_str());
            }

            if(ui->cursorDown_v1->isDown() && remote_speed_v1_ >= 0) {
                remote_speed_v1_ -= 0.1f;

                if( remote_speed_v1_ < 0 ) {
                    remote_speed_v1_ = 0;
                }

                ui->info_remote_speed_v1->insert( std::to_string( remote_speed_v1_).c_str());
            }

            if(ui->cursorLeft_v1->isDown() && remote_lat_angle_v1_ > -70 ) {
                remote_lat_angle_v1_ -= 3;
                ui->info_remote_lat_v1->insert( std::to_string( remote_lat_angle_v1_).c_str());
            }

            if(ui->cursorRight_v1->isDown() && remote_lat_angle_v1_ < 70 ) {
                remote_lat_angle_v1_ += 3;
                ui->info_remote_lat_v1->insert( std::to_string( remote_lat_angle_v1_).c_str());
            }

            platooning::remotecontrolInput msg;

            msg.remote_speed = remote_speed_v1_;
            msg.remote_angle = remote_lat_angle_v1_;
            msg.emergency_stop = false;
            server_ptr_->start_send(platooning::MessageTypes::encode_message(msg),REMOTE_CONTROLINPUT);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1500));
        }

        while(remoteEnabled_v2_ && tabix == 1) {
            if(ui->cursorUp_v2->isDown() && remote_speed_v2_ < 2.8) {
                remote_speed_v2_ += 0.1f;

                ui->info_remote_speed_v2->insert( std::to_string( remote_speed_v2_).c_str());
            }

            if(ui->cursorDown_v2->isDown() && remote_speed_v2_ >= 0) {
                remote_speed_v2_ -= 0.1f;

                if( remote_speed_v2_ < 0 ) {
                    remote_speed_v2_ = 0;
                }

                ui->info_remote_speed_v2->insert( std::to_string( remote_speed_v2_).c_str());
            }

            if(ui->cursorLeft_v2->isDown() && remote_lat_angle_v2_ > -70 ) {
                remote_lat_angle_v2_ -= 3;
                ui->info_remote_lat_v2->insert( std::to_string( remote_lat_angle_v2_).c_str());
            }

            if(ui->cursorRight_v2->isDown() && remote_lat_angle_v2_ < 70 ) {
                remote_lat_angle_v2_ += 3;
                ui->info_remote_lat_v2->insert( std::to_string( remote_lat_angle_v2_).c_str());
            }

            platooning::remotecontrolInput msg;

            msg.remote_speed = remote_speed_v2_;
            msg.remote_angle = remote_lat_angle_v2_;
            msg.emergency_stop = false;
            server_ptr_->start_send(platooning::MessageTypes::encode_message(msg),REMOTE_CONTROLINPUT);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1500));
        }

        while(remoteEnabled_v3_ && tabix == 1) {
            if(ui->cursorUp_v3->isDown() && remote_speed_v3_ < 2.8) {
                remote_speed_v3_ += 0.1f;

                ui->info_remote_speed_v3->insert( std::to_string( remote_speed_v3_).c_str());
            }

            if(ui->cursorDown_v3->isDown() && remote_speed_v3_ >= 0) {
                remote_speed_v3_ -= 0.1f;

                if( remote_speed_v3_ < 0 ) {
                    remote_speed_v3_ = 0;
                }

                ui->info_remote_speed_v3->insert( std::to_string( remote_speed_v3_).c_str());
            }

            if(ui->cursorLeft_v3->isDown() && remote_lat_angle_v3_ > -70 ) {
                remote_lat_angle_v3_ -= 3;
                ui->info_remote_lat_v3->insert( std::to_string( remote_lat_angle_v3_).c_str());
            }

            if(ui->cursorRight_v3->isDown() && remote_lat_angle_v3_ < 70 ) {
                remote_lat_angle_v3_ += 3;
                ui->info_remote_lat_v3->insert( std::to_string( remote_lat_angle_v3_).c_str());
            }

            platooning::remotecontrolInput msg;

            msg.remote_speed = remote_speed_v3_;
            msg.remote_angle = remote_lat_angle_v3_;
            msg.emergency_stop = false;
            server_ptr_->start_send(platooning::MessageTypes::encode_message(msg),REMOTE_CONTROLINPUT);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1500));
        }

    } catch( std::exception &ex) {
        std::cerr << "keypresspoll crash with " << ex.what() << std::endl;
    }
}

void ControllerUi::keyPressEvent(QKeyEvent *event)
{
    if(!remoteEnabled_v1_ && !remoteEnabled_v2_ && !remoteEnabled_v3_) {
        return;
    }

    int tabix = ui->tabWidget->currentIndex();

    if(event->key() == Qt::Key_Left ) {
        if( tabix == 0 ) {
            ui->cursorLeft_v1->setDown(true);
        }

        if( tabix == 1 ) {
            ui->cursorLeft_v2->setDown(true);
        }

        if( tabix == 2 ) {
            ui->cursorLeft_v3->setDown(true);
        }
    }

    if(event->key() == Qt::Key_Right ) {
        if( tabix == 0 ) {
            ui->cursorRight_v1->setDown(true);
        }

        if( tabix == 1 ) {
            ui->cursorRight_v2->setDown(true);
        }

        if( tabix == 2 ) {
            ui->cursorRight_v3->setDown(true);
        }
    }

    if(event->key() == Qt::Key_Up ) {
        if( tabix == 0 ) {
            ui->cursorUp_v1->setDown(true);
        }

        if( tabix == 1 ) {
            ui->cursorUp_v2->setDown(true);
        }

        if( tabix == 2 ) {
            ui->cursorUp_v3->setDown(true);
        }

    }

    if(event->key() == Qt::Key_Down ) {
        if( tabix == 0 ) {
            ui->cursorDown_v1->setDown(true);
        }

        if( tabix == 1 ) {
            ui->cursorDown_v2->setDown(true);
        }

        if( tabix == 2 ) {
            ui->cursorDown_v3->setDown(true);
        }

    }
}

void ControllerUi::keyReleaseEvent(QKeyEvent *event)
{
    if(!remoteEnabled_v1_ && !remoteEnabled_v2_ && !remoteEnabled_v3_) {
        return;
    }

    int tabix = ui->tabWidget->currentIndex();

    if(event->key() == Qt::Key_Left ) {
        if( tabix == 0 ) {
            ui->cursorLeft_v1->setDown(false);
        }

        if( tabix == 1 ) {
            ui->cursorLeft_v2->setDown(false);
        }

        if( tabix == 2 ) {
            ui->cursorLeft_v3->setDown(false);
        }
    }

    if(event->key() == Qt::Key_Right ) {
        if( tabix == 0 ) {
            ui->cursorRight_v1->setDown(false);
        }

        if( tabix == 1 ) {
            ui->cursorRight_v2->setDown(false);
        }

        if( tabix == 2 ) {
            ui->cursorRight_v3->setDown(false);
        }
    }

    if(event->key() == Qt::Key_Up ) {
        if( tabix == 0 ) {
            ui->cursorUp_v1->setDown(false);
        }

        if( tabix == 1 ) {
            ui->cursorUp_v2->setDown(false);
        }

        if( tabix == 2 ) {
            ui->cursorUp_v3->setDown(false);
        }

    }

    if(event->key() == Qt::Key_Down ) {
        if( tabix == 0 ) {
            ui->cursorDown_v1->setDown(false);
        }

        if( tabix == 1 ) {
            ui->cursorDown_v2->setDown(false);
        }

        if( tabix == 2 ) {
            ui->cursorDown_v3->setDown(false);
        }

    }
}
