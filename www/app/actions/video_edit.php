<?php

$this['video'] = 'active';

$id = Atomik::get('request.id');

// In case of an edit
if($_POST){
	// validation of data
	$fields = array(
		'name' => array('required' => true ),
		'description' => array('required' => true, 'message' => 'You must provide a desciption.'),
		'url' => array('required' => true),
		'id' => array('required' => true, 'filter' => FILTER_VALIDATE_INT)
		);


	if (($data = $this->filter($_POST, $fields)) === false) {
	    $this->flash($this['app.filters.messages'], 'error');
	   
	}else{

		if($data['id'] == -1){
			// new video
			$data['id'] = NULL;
			$this['db']->insert('videos', $data);
			$id = $this['db']->lastInsertId ("id");
		}else{
			// update of a video
			$this['db']->update('videos', $data, array('id' => $data['id']));
			$id = $data['id'];
		}
	$this->flash('Video succesfuly edited.', 'success');
	}

}


if($id != -1){
	$video = $this['db']->selectOne('videos', array('id' => $id));
}else{
	$video = array(
		'name' 			=> '',
		'description' 	=> '',
		'url'			=> ''
		);
}


