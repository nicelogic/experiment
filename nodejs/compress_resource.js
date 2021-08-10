


var fs = require('fs');
var file_system = require('fs');
var archiver = require('archiver');
let shellJs = require('shelljs');



const fileDir = '/Users/bryan.wu/Downloads/banuba-upgrade/fixed_landscape_07.30';
/*dir must contain has umcompress files
	will check 
		1. *.ogg *.aac 
		2. config.json
		3. config.js
*/

fs.readdir(fileDir, function (err, files) {
	if (err) {
		return console.log('目录不存在');
	}

	files.forEach((fileName) => {
		if(fileName.indexOf('.DS_Store') != -1){
			return;
		}

		const subDir = fileDir + '/' + fileName;
		fs.stat(subDir, function (err, stat) {
			if (stat && stat.isDirectory()) {
				fs.readdir(subDir, function (err, subDirFiles) {
					subDirFiles.forEach((subDirFileName) => {
						if(subDirFileName.indexOf('.ogg') != -1){
							console.log(`${fileName} has ogg file`);
						}
						if(subDirFileName.indexOf('.aac') != -1){
							console.log(`${fileName} has aac file`);
						}
					});
				});
			}
		});

		const effectDir = fileDir + '/' + fileName;
		const dsFile = effectDir + '/.DS_Store';
		console.log(`remove ${dsFile}`);
		shellJs.rm('-rf', dsFile);

		const zipName = effectDir + '.zip';
		var output = file_system.createWriteStream(zipName);
		var archive = archiver('zip');
		output.on('close', function () {
			console.log(archive.pointer() + ' total bytes');
			console.log('archiver has been finalized and the output file descriptor has closed.');
		});
		archive.on('error', function (err) {
			throw err;
		});
		archive.pipe(output);
		archive.directory(effectDir, '');
		archive.finalize();
	});
});

