<?php
$this['scene'] = 'active';

if($_POST){
	// validation of data
	$fields = array(
	    'id_scene' => array('required' => true, 'filter' => FILTER_VALIDATE_INT),
	);

	if (($data = $this->filter($_POST, $fields)) === false) {
	    $this->flash($this['app.filters.messages'], 'error');
	}else{
		// remove device
		$this['db']->delete('scenes', array('id' => $data['id_scene']));
		
		$this->flash('Device successfully deleted!', 'success');
	}
}

$stmt = $this['db']->prepare('SELECT `scenes`.*, `videos`.`name` as `video_name` FROM `scenes` LEFT OUTER JOIN `videos` ON `scenes`.`id_video` = `videos`.`id`');
$stmt->execute();
$scenes = $stmt->fetchAll();

