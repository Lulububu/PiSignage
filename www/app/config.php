<?php




Atomik::set(array(

    'plugins' => array(
        'DebugBar' => array(
            // if you don't include jquery yourself as it is done in the
            // skeleton, comment out this line (the debugbar will include jquery)
            'include_vendors' => 'css'
        ),
       
        'Session',
        'Flash'
    ),

    'app.layout' => '_layout',

    'atomik.url_rewriting' => true,


    // WARNING: change this to false when in production
    'atomik.debug' => true
    
));



Atomik::set('plugins.Db', array(
    'dsn' => 'mysql:host=localhost;dbname=PiSignage',
    'username' => 'root',
    'password' => 'root'
));


Atomik::set('app.routes', array(
   'scene' => array(
       'action' => 'scene_list'
   ),
   'scene/edit/:id' => array(
       'action' => 'scene_edit',
       'id' => '-1'
   ),
   'device' => array(
       'action' => 'device_list',
   ),
   'device/edit/:id' => array(
       'action' => 'device_edit',
       'id' => '-1'
   ),
   'video' => array(
       'action' => 'video_list',
   ),
   'video/edit/:id' => array(
       'action' => 'video_edit',
       'id' => '-1'
   )
));