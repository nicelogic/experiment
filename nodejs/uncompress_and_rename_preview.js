

// import UrlParse from 'url-parse';
// const urlParser = new UrlParse(window.location.href, true);

let fs = require('fs')
let AdmZip = require('adm-zip');
let shellJs = require('shelljs');

const fileDir = '/Users/bryan.wu/Downloads/banuba-upgrade/fixed_landscape_07.30';

fs.readdir(fileDir, function (err, files) {
	if (err) {
		return console.log('目录不存在');
	}
	files.forEach((fileName) => {
		if (fileName.indexOf('.zip') == -1) {
			console.log(fileName);

			const removeDir = fileDir + '/' + fileName;
			shellJs.rm('-rf', removeDir);
			// fs.rmdir(removeDir, (error) => {
			// 	if (error) {
			// 		console.log(error);
			// 	} else {

			// 		console.log(`remove ${removeDir}`);
			// 	}
			// });
			return;
		}

		// if (fileName != "2_5D_AngelLow-05.01-1.005.086-3023ebf8af.zip") {
		// 	return;
		// }



		const zipFile = fileDir + '/' + fileName;
		console.log(zipFile);

		const name = fileName.substr(0, fileName.lastIndexOf('.'));
		const extractDir = fileDir + '/' + name;
		const errorExtractDir = extractDir + '/' + name
		let zip = new AdmZip(zipFile);
		zip.extractAllTo(extractDir, true);
		fs.stat(errorExtractDir, function (err, stat) {
			if (stat && stat.isDirectory()) {
				console.log(`dir ${errorExtractDir} exist`);
				console.log(`extradir ${extractDir}`);
				const backDir = extractDir + '-back';
				console.log('ok');
				shellJs.cp('-rf', errorExtractDir, backDir);
				shellJs.rm('-rf', extractDir);
				shellJs.mv(backDir, extractDir);
			}

			const thumbanilName = extractDir + '/' + name + '.png';
			const prewName = extractDir + '/preview.png';
			fs.rename(prewName, thumbanilName, (error) => {
				if (error) {
					console.log(error);
				} else {
					console.log('ok');
				}
			});

			shellJs.cp('-f', thumbanilName, fileDir)
			shellJs.rm('-rf', extractDir);
		});

	});
});
